#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastrtps/transport/TCPv4TransportDescriptor.h>
#include <fastrtps/transport/UDPv4TransportDescriptor.h>

#include "DdsPublisher.h"

using namespace eprosima::fastdds::dds;
using eprosima::fastrtps::types::ReturnCode_t;

PublisherService::PublisherService(const ServiceConfig<PublisherConfig>& config)
	: participant_(nullptr)
	, config_(config)
	, stop(false)
{
}

PublisherService::PublisherService()
	: participant_(nullptr)
	, stop(false)
{
}

PublisherService::~PublisherService()
{
	deletePublishers();
	// Без этого иногда на SUB не изменяется SubscriptionMatchedStatus
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	participant_->delete_contained_entities();
	if (DomainParticipantFactory::get_instance()->delete_participant(participant_) != ReturnCode_t::RETCODE_OK)
	{
		std::cout << "Participant deletion failed" << std::endl;
	}
}

bool PublisherService::initPublishers()
{
	using namespace eprosima::fastrtps;
	//TODO где вызывать?
	setVectorSizesInDataTopic();

	createParticipant();

	if (!config_.configs.empty())
	{
		for (const auto& config : config_.configs)
		{
			createNewPublisher(config);
		}
	}
	else
	{
		std::cout << "Configuration for publishers is not found" << std::endl;
		return false;
	}
	for (auto& pub : publishers_)
	{
		pub->init();
	}
	return true;
}

eprosima::fastdds::dds::DomainParticipantQos PublisherService::getParticipantQos()
{
	using namespace eprosima::fastrtps;
	using namespace eprosima::fastrtps::rtps;
	//using namespace eprosima::fastdds::rtps;
	DomainParticipantQos qos;
	qos.name(config_.participant_name);

	qos.wire_protocol().builtin.discovery_config.leaseDuration = c_TimeInfinite;
	qos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = Duration_t(1, 0);

	qos.transport().use_builtin_transports = false;

	if (config_.transport = Transport::TCP)
	{
		std::shared_ptr<TCPv4TransportDescriptor> descriptor = std::make_shared<TCPv4TransportDescriptor>();

		/*std::vector<std::string> whitelist(config_.whitelist);
		for (std::string ip : whitelist)
		{
			descriptor->interfaceWhiteList.push_back(ip);
			std::cout << "Whitelisted " << ip << std::endl;
		}*/

		descriptor->sendBufferSize = 0;
		descriptor->receiveBufferSize = 0;

		descriptor->set_WAN_address(config_.ip);

		descriptor->add_listener_port(config_.port);

		qos.transport().user_transports.push_back(descriptor);
	}
	else
	{
		std::shared_ptr<UDPv4TransportDescriptor> descriptor = std::make_shared<UDPv4TransportDescriptor>();
		
		descriptor->sendBufferSize = 0;
		descriptor->receiveBufferSize = 0;
		descriptor->non_blocking_send = true;

		qos.transport().user_transports.push_back(descriptor);
	}
	
	return qos;
}

bool PublisherService::createParticipant()
{
	if (participant_ == nullptr)
	{
		std::cout << "Participant is creating with transport IP " << config_.ip << " and port " << config_.port << std::endl;
		DomainParticipantQos qos(getParticipantQos());
		participant_ = DomainParticipantFactory::get_instance()->create_participant(0, qos);
		if (participant_ == nullptr)
		{
			std::cout << "participant is nullptr" << std::endl;
			return false;
		}
		std::cout << "Participant created" << std::endl;
	}
	return true;
}

void PublisherService::runPublishers()
{
	std::vector<std::thread> threads;
	for (auto& pub : publishers_)
	{
		threads.push_back(std::thread([&](){pub->run();}));
	}
	for (auto& t : threads)
	{
		t.join();
	}
}

void PublisherService::changeSubsConfigAndInit(const ServiceConfig<PublisherConfig>& config)
{
	if (config_ == config)
	{
		std::cout << "This subscriber's configuration has been already run" << std::endl;
	}
	else
	{
		config_ = config;
		deletePublishers();
		initPublishers();
	}
}

void PublisherService::setData()
{
	for (auto pub : publishers_)
	{
		pub->setData();
	}
}

void PublisherService::setDdsData(DDSData* data)
{
	for (auto pub : publishers_)
	{
		if (pub->getTopicType() == TopicType::DDS_DATA)
		{
			pub->setData(static_cast<void*>(data));
		}
	}
}

void PublisherService::setDdsDataEx(DDSDataEx* data)
{
	for (auto pub : publishers_)
	{
		if (pub->getTopicType() == TopicType::DDS_DATA_EX)
		{
			pub->setData(static_cast<void*>(data));
		}
	}
}

void setBenchmarkSimple(BenchmarkSimple* data, size_t size)
{

}

bool PublisherService::createNewPublisher(const PublisherConfig& config)
{
	// TODO: узнать че менять в SUBSCRIBER_QOS_DEFAULT
	AbstractDdsPublisher* pub = factory_.createPublisher(participant_, config);
	if (pub == nullptr)
	{
		return false;
	}

	std::lock_guard<std::mutex> guard(std::mutex());
	publishers_.push_back(pub);
	return true;
}


void PublisherService::setVectorSizesInDataTopic()
{
	scada_ate::typetopics::SetMaxSizeDataChar(config_.MaxSizeDataCollectionInt);
	scada_ate::typetopics::SetMaxSizeDataCollectionInt(config_.MaxSizeDataCollectionInt);
	scada_ate::typetopics::SetMaxSizeDataCollectionFloat(config_.MaxSizeDataCollectionFloat);
	scada_ate::typetopics::SetMaxSizeDataCollectionDouble(config_.MaxSizeDataCollectionDouble);
	scada_ate::typetopics::SetMaxSizeDataCollectionChar(config_.MaxSizeDataCollectionChar);

	scada_ate::typetopics::SetMaxSizeDataExVectorChar(config_.MaxSizeDDSDataExVectorInt);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorInt(config_.MaxSizeDDSDataExVectorInt);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorFloat(config_.MaxSizeDDSDataExVectorFloat);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorDouble(config_.MaxSizeDDSDataExVectorDouble);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorChar(config_.MaxSizeDDSDataExVectorChar);

	scada_ate::typetopics::SetMaxSizeDDSAlarmExVectorAlarms(config_.MaxSizeDDSAlarmVectorAlarm);

	scada_ate::typetopics::SetMaxSizeDDSAlarmExVectorAlarms(config_.MaxSizeDDSExVectorAlarms);
}

void PublisherService::deletePublishers()
{
	for (auto& pub : publishers_)
	{
		delete pub;
	}
	publishers_.clear();
}