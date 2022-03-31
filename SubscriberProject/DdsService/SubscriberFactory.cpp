#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include "SubscriberFactory.h"

using namespace eprosima::fastdds::dds;

bool operator==(const SubscriberConfig& lhs, const SubscriberConfig& rhs) 
{
	return lhs.subscriber_id == rhs.subscriber_id
		&& lhs.vector_size == rhs.subscriber_id
		&& lhs.topic_name == rhs.topic_name
		&& lhs.topic_type_name == rhs.topic_type_name
		&& lhs.topic_type == rhs.topic_type;
}

template<>
void ConcreteSubscriber<DDSData, DDSDataPubSubType>::setDataSize()
{
	std::vector<int> v(10, 0);
	DataCollectionInt dataCollectionInt;
	dataCollectionInt.value(v);
	data_sample_.data_int(dataCollectionInt);
}

template<>
void ConcreteSubscriber<DDSData, DDSDataPubSubType>::runDataSending()
{
}

template<>
void ConcreteSubscriber<BenchmarkSimple, BenchmarkSimplePubSubType>::SubscriberListener::specificDataHandling(BenchmarkSimple& data)
{
	PacketInfo packet_info{
						data_sample_.time_service(),
						TimeConverter::GetTime_LLmcs(),
						packet_info.receiving_time - packet_info.dispatch_time,
						BenchmarkSimple::getCdrSerializedSize(data_sample_)
	};
	std::cout << "Sample #" << data.index()
		<< " | Delivery time: " << packet_info.delivery_time
		<< " | Transmitted size: " << packet_info.size << std::endl;
	if (!sub_->data_.empty() && data.index() - sub_->data_.back().second.index() > 1)
	{
		++losted_count_;
		std::cout << "Previous package is losted" << std::endl;
	}
	sub_->data_.push_back(std::make_pair(packet_info, std::move(data_sample_)));
}

template<>
void ConcreteSubscriber<DDSData, DDSDataPubSubType>::SubscriberListener::specificDataHandling(DDSData& data)
{
	PacketInfo packet_info{
						data_sample_.time_service(),
						TimeConverter::GetTime_LLmcs(),
						packet_info.receiving_time - packet_info.dispatch_time,
						DDSData::getCdrSerializedSize(data_sample_)
	};
	std::cout << "Delivery time: " << packet_info.delivery_time
		<< " | Transmitted size: " << packet_info.size << std::endl;
	sub_->data_.push_back(std::make_pair(packet_info, std::move(data_sample_)));
}

template<>
void ConcreteSubscriber<DDSDataEx, DDSDataExPubSubType>::SubscriberListener::specificDataHandling(DDSDataEx& data)
{
}

template<>
void ConcreteSubscriber<DDSAlarm, DDSAlarmPubSubType>::SubscriberListener::specificDataHandling(DDSAlarm& data)
{
}

template<>
void ConcreteSubscriber<DDSAlarmEx, DDSAlarmExPubSubType>::SubscriberListener::specificDataHandling(DDSAlarmEx& data)
{
}

AbstractDdsSubscriber* SubscriberFactory::createSubscriber(
	eprosima::fastdds::dds::DomainParticipant* participant,
	const SubscriberConfig& config) const
{
	switch (config.topic_type)
	{
	case TopicType::DDS_DATA:
		return new ConcreteSubscriber<DDSData, DDSDataPubSubType>(participant, config);
	case TopicType::DDS_DATA_EX:
		return new ConcreteSubscriber<DDSDataEx, DDSDataExPubSubType>(participant, config);
	case TopicType::DDS_ALARM:
		return new ConcreteSubscriber<DDSAlarm, DDSAlarmPubSubType>(participant, config);
	case TopicType::DDS_EX_ALARM:
		return new ConcreteSubscriber<DDSAlarmEx, DDSAlarmExPubSubType>(participant, config);
	case TopicType::BENCHMARK_SIMPLE:
		return new ConcreteSubscriber<BenchmarkSimple, BenchmarkSimplePubSubType>(participant, config);
	default:
		std::cout << "Topic type " << config.topic_type_name << " is not found" << std::endl;
		return nullptr;
	}
}