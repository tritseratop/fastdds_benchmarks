#include <mutex>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastrtps/Domain.h>
#include <fastrtps/transport/TCPv4TransportDescriptor.h>
#include <fastrtps/utils/IPLocator.h>

#include "DdsSubscriber.h"

using namespace eprosima::fastdds::dds;
using eprosima::fastrtps::types::ReturnCode_t;

SubscriberService::SubscriberService(const ServiceConfig<SubscriberConfig>& config, IServer* server)
	: participant_(nullptr)
	, config_(config)
	, stop_ws_server_(false)
{
	//TODO где вызывать?
	setVectorSizesInDataTopic();
}

SubscriberService::~SubscriberService()
{
	deleteSubscribers();
	std::cout << "All subscriber deleted" << std::endl;
	if (participant_->delete_contained_entities() != ReturnCode_t::RETCODE_OK)
	{
		std::cout << "Participant entities deletion failed" << std::endl;
	}
	std::cout << "Participant entities deleted" << std::endl;
	if (DomainParticipantFactory::get_instance()->delete_participant(participant_) != ReturnCode_t::RETCODE_OK)
	{
		std::cout << "Participant deletion failed" << std::endl;
	}
	std::cout << "Sub service deleted" << std::endl;
}

void SubscriberService::changeSubsConfig(const ServiceConfig<SubscriberConfig>& config)
{
	if (config_ == config)
	{
		std::cout << "This subscriber's configuration has been already runConfigPub" << std::endl;
	}
	else
	{
		config_ = config;
		deleteSubscribers();
		initSubscribers();
	}
}

bool SubscriberService::createParticipant()
{
	if (participant_ == nullptr)
	{
		std::cout << "Participant is creating with transport IP " << config_.ip << " and port " << config_.port << std::endl;
		participant_ = DomainParticipantFactory::get_instance()->create_participant(0, getParticipantQos());
	}
	if (participant_ == nullptr)
	{
		return false;
	}
	std::cout << "Participant created" << std::endl;
	return true;
}


DomainParticipantQos SubscriberService::getParticipantQos()
{
	using namespace eprosima::fastrtps;
	using namespace eprosima::fastrtps::rtps;

	DomainParticipantQos qos;

	Locator_t initial_peer_locator;
	initial_peer_locator.kind = LOCATOR_KIND_TCPv4;

	std::shared_ptr<TCPv4TransportDescriptor> descriptor = std::make_shared<TCPv4TransportDescriptor>();

	/*for (std::string ip : config_.whitelist)
	{
		descriptor->interfaceWhiteList.push_back(ip);
		std::cout << "Whitelisted " << ip << std::endl;
	}*/
	IPLocator::setIPv4(initial_peer_locator, config_.ip);

	initial_peer_locator.port = config_.port;
	qos.wire_protocol().builtin.initialPeersList.push_back(initial_peer_locator); // Publisher's meta channel

	qos.wire_protocol().builtin.discovery_config.leaseDuration = c_TimeInfinite;
	qos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = Duration_t(1, 0);
	qos.name(config_.participant_name);
	

	qos.transport().use_builtin_transports = false;

	qos.transport().user_transports.push_back(descriptor);

	return qos;
}

bool SubscriberService::initSubscribers()
{
	createParticipant();
	if (!config_.configs.empty())
	{
		for (const auto& config : config_.configs)
		{
			createNewSubscriber(config);
		}
	}
	else
	{
		std::cout << "Configuration for subscribers is not found" << std::endl;
		return false;
	}
	for (auto& sub : subscribers_)
	{
		sub->init();
	}
	return true;
}

bool SubscriberService::createNewSubscriber(const SubscriberConfig& config)
{
	// TODO: узнать че менять в SUBSCRIBER_QOS_DEFAULT
	AbstractDdsSubscriber* sub = factory_.createSubscriber(participant_, config);
	if (sub == nullptr)
	{
		return false;
	}

	std::lock_guard<std::mutex> guard(std::mutex());
	subscribers_.push_back(sub);
	return true;
}

void SubscriberService::runSubscribers()
{
	std::vector<std::thread> threads;
	for (auto& sub : subscribers_)
	{
		threads.push_back( std::thread([&](){sub->run(); }) );
	}

	for (auto& t : threads)
	{
		t.join();
	}
}

std::vector<AbstractDdsSubscriber*> SubscriberService::getSubscribers() const
{
	return subscribers_;
}

void SubscriberService::deleteSubscribers()
{
	for (auto& sub : subscribers_)
	{
		delete sub;
	}
	subscribers_.clear();
}

// TODO: сделать макрос?
void SubscriberService::setVectorSizesInDataTopic()
{
	scada_ate::typetopics::SetMaxSizeDataCollectionInt(config_.MaxSizeDataCollectionInt);
	scada_ate::typetopics::SetMaxSizeDataCollectionFloat(config_.MaxSizeDataCollectionFloat);
	scada_ate::typetopics::SetMaxSizeDataCollectionDouble(config_.MaxSizeDataCollectionDouble);
	scada_ate::typetopics::SetMaxSizeDataCollectionChar(config_.MaxSizeDataCollectionChar);

	scada_ate::typetopics::SetMaxSizeDDSDataExVectorInt(config_.MaxSizeDDSDataExVectorInt);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorFloat(config_.MaxSizeDDSDataExVectorFloat);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorDouble(config_.MaxSizeDDSDataExVectorDouble);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorChar(config_.MaxSizeDDSDataExVectorChar);

	scada_ate::typetopics::SetMaxSizeDDSAlarmExVectorAlarms(config_.MaxSizeDDSAlarmVectorAlarm);

	scada_ate::typetopics::SetMaxSizeDDSAlarmExVectorAlarms(config_.MaxSizeDDSExVectorAlarms);
}