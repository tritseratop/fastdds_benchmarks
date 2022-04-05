#include "PublisherFactory.h"
#include "../include/TestUtility.h"

using namespace eprosima::fastdds::dds;

bool operator==(const PublisherConfig& lhs, const PublisherConfig& rhs)
{
	return lhs.subscriber_id == rhs.subscriber_id
		&& lhs.vector_size == rhs.subscriber_id
		&& lhs.topic_name == rhs.topic_name
		&& lhs.topic_type_name == rhs.topic_type_name
		&& lhs.topic_type == rhs.topic_type;
}

template<>
void ConcretePublisher<BenchmarkSimple, BenchmarkSimplePubSubType>::specificDataChanging()
{
	data_.index(samples_count_);
}

template<>
void ConcretePublisher<BenchmarkVector, BenchmarkVectorPubSubType>::specificDataChanging()
{
	if (data_.data().empty())
	{
		data_.data().resize(10);
	}

	if (samples_count_ >= config_.samples)
	{
		data_.data().resize(data_.data().size() * 10);
	}
	data_.index(samples_count_);
	
}

template<>
void ConcretePublisher<DDSData, DDSDataPubSubType>::specificDataChanging()
{
}

template<>
void ConcretePublisher<DDSDataEx, DDSDataExPubSubType>::specificDataChanging()
{
}

template<>
void ConcretePublisher<DDSAlarm, DDSAlarmPubSubType>::specificDataChanging()
{
}

template<>
void ConcretePublisher<DDSAlarmEx, DDSAlarmExPubSubType>::specificDataChanging()
{
}

template<>
void ConcretePublisher<BenchmarkSimple, BenchmarkSimplePubSubType>::setData()
{
}

template<>
void ConcretePublisher<BenchmarkVector, BenchmarkVectorPubSubType>::setData()
{
}

template<>
void ConcretePublisher<DDSData, DDSDataPubSubType>::setData()
{
	data_ = getDdsData(config_.vector_size);
}

template<>
void ConcretePublisher<DDSDataEx, DDSDataExPubSubType>::setData()
{
	data_ = getDdsDataEx(config_.vector_size);
}

template<>
void ConcretePublisher<DDSAlarm, DDSAlarmPubSubType>::setData()
{
}

template<>
void ConcretePublisher<DDSAlarmEx, DDSAlarmExPubSubType>::setData()
{
}

AbstractDdsPublisher* PublisherFactory::createPublisher(
	eprosima::fastdds::dds::DomainParticipant* participant,
	const PublisherConfig& config) const
{
	switch (config.topic_type)
	{
	case TopicType::DDS_DATA:
		return new ConcretePublisher<DDSData, DDSDataPubSubType>(participant, config);
	case TopicType::DDS_DATA_EX:
		return new ConcretePublisher<DDSDataEx, DDSDataExPubSubType>(participant, config);
	case TopicType::DDS_ALARM:
		return new ConcretePublisher<DDSAlarm, DDSAlarmPubSubType>(participant, config);
	case TopicType::DDS_EX_ALARM:
		return new ConcretePublisher<DDSAlarmEx, DDSAlarmExPubSubType>(participant, config);
	case TopicType::BENCHMARK_SIMPLE:
		return new ConcretePublisher<BenchmarkSimple, BenchmarkSimplePubSubType>(participant, config);
	case TopicType::BENCHMARK_VECTOR:
		return new ConcretePublisher<BenchmarkVector, BenchmarkVectorPubSubType>(participant, config);
	default:
		std::cout << "Topic type " << config.topic_type_name << " is not found" << std::endl;
		return nullptr;
	}
}