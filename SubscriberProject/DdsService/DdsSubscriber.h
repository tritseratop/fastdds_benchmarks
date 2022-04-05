#ifndef DDS_SUBSCRIBER_H_
#define DDS_SUBSCRIBER_H_

#include <unordered_map>
#include <vector>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include <fastrtps/attributes/ParticipantAttributes.h>

#include "../../TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "SubscriberFactory.h"

class SubscriberService {
public: 
	SubscriberService(const ServiceConfig<SubscriberConfig>& config, IServer* server);
	SubscriberService();
	virtual ~SubscriberService();

	void changeSubsConfigAndInit(const ServiceConfig<SubscriberConfig>& config);

	bool createParticipant();
	eprosima::fastdds::dds::DomainParticipantQos getParticipantQos();
	bool initSubscribers();
	bool createNewSubscriber(const SubscriberConfig& config);
	void runSubscribers();
	void runWsServer();
	std::vector<AbstractDdsSubscriber*> getSubscribers() const;
	void setVectorSizesInDataTopic();

	std::deque<DataDto> getDataCacheCopy() const;
	void deleteSubscribers();

private:

	bool stop_ws_server_;

	ServiceConfig<SubscriberConfig> config_;

	SubscriberFactory factory_;

	// Контейнеры с пользовательскими типам
	std::vector<AbstractDdsSubscriber*> subscribers_;

	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::TypeSupport config_type_; // TODO не нужна как поле 

};

#endif //!DDS_SUBSCRIBER_H_