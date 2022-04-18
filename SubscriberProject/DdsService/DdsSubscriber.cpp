#include <mutex>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastrtps/Domain.h>
#include <fastrtps/transport/TCPv4TransportDescriptor.h>
#include <fastrtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/utils/IPLocator.h>

#include "DdsSubscriber.h"

using namespace eprosima::fastdds::dds;
using eprosima::fastrtps::types::ReturnCode_t;

SubscriberService::SubscriberService(const ServiceConfig<SubscriberConfig>& config, IServer* server)
	: participant_(nullptr)
	, stop_ws_server_(false)
	, config_(config)
{
	log = logger::Logger::getInstance("Subscriber", "logs.txt", true);
}

SubscriberService::SubscriberService()
	: participant_(nullptr)
	, stop_ws_server_(false)
{
	log = logger::Logger::getInstance("Subscriber", "logs.txt", true);
}

SubscriberService::~SubscriberService()
{
	deleteSubscribers();
	*log << logger::Logger::e_logType::LOG_INFO << "All subscriber deleted";
	if (participant_->delete_contained_entities() != ReturnCode_t::RETCODE_OK)
	{
		*log << logger::Logger::e_logType::LOG_ERROR << "Participant's entities deletion failed";
	}
	std::cout << "Participant entities deleted" << std::endl;
	if (DomainParticipantFactory::get_instance()->delete_participant(participant_) != ReturnCode_t::RETCODE_OK)
	{
		*log << logger::Logger::e_logType::LOG_ERROR << "Participant deletion failed";
	}
	*log << logger::Logger::e_logType::LOG_INFO << "Sub service deleted";
}

void SubscriberService::changeSubsConfigAndInit(const ServiceConfig<SubscriberConfig>& config)
{
	if (config_ == config)
	{
		*log << logger::Logger::e_logType::LOG_INFO << "This subscriber's configuration has been already runConfigPub";
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
		*log << logger::Logger::e_logType::LOG_INFO
			<< "Participant is creating with transport IP " + config_.ip + " and port " + std::to_string(config_.port);
		participant_ = DomainParticipantFactory::get_instance()->create_participant(0, getParticipantQos());
		if (participant_ == nullptr)
		{
			return false;
		}
		*log << logger::Logger::e_logType::LOG_INFO << "Participant created";
	}
	return true;
}


DomainParticipantQos SubscriberService::getParticipantQos()
{
	using namespace eprosima::fastrtps;
	using namespace eprosima::fastrtps::rtps;

	DomainParticipantQos qos;

	if (config_.transport = Transport::TCP)
	{
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

bool SubscriberService::initSubscribers()
{
	setVectorSizesInDataTopic();
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
		*log << logger::Logger::e_logType::LOG_WARNING << "Configuration for subscribers is not found";
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