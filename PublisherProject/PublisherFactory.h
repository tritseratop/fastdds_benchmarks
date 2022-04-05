#ifndef PUBLISHER_FACTORY_H_
#define PUBLISHER_FACTORY_H_

#include <thread>
#include <mutex>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include <fastrtps/transport/TCPv4TransportDescriptor.h>

#include "../TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "../BenchmarkTopics/BenchmarkSimplePubSubTypes.h"
#include "../BenchmarkTopics/BenchmarkVectorPubSubTypes.h"
#include "../include/TimeConverter/TimeConverter.hpp"
#include "../include/CommonClasses.h"

using eprosima::fastrtps::types::ReturnCode_t;

struct PublisherConfig
{
	int16_t subscriber_id = 0;
	uint16_t vector_size = 0;
	std::string topic_name = "";
	std::string topic_type_name = "";
	TopicType topic_type = TopicType::UNKNOWN;

	// listener settings
	uint32_t samples = 10;
	uint32_t sleep = 1000;

	friend bool operator==(const PublisherConfig& lhs, const PublisherConfig& rhs);
};

class AbstractDdsPublisher
{
public:
	virtual ~AbstractDdsPublisher() {};
	virtual bool init() = 0;
	virtual void run() = 0;
	virtual void setData(void* data) = 0;
	virtual void setData() = 0;
	virtual TopicType getTopicType() = 0;
	//virtual void setConfig(const SubscriberConfig& config) = 0;
protected:
};

template <class T, class TPubSubType>
class ConcretePublisher : public AbstractDdsPublisher
{
public:
	ConcretePublisher(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const PublisherConfig& config
	)
		: participant_(participant)
		, config_(config)
		, publisher_(nullptr)
		, topic_(nullptr)
		, writer_(nullptr)
		, type_(new TPubSubType())
		, listener_(this)
		, topic_type_(config.topic_type)
		, stop_(false)
		, samples_count_(1)
	{
	}

	~ConcretePublisher() override
	{
		if (writer_ != nullptr)
		{
			auto res = publisher_->delete_datawriter(writer_);
			if (res != ReturnCode_t::RETCODE_OK)
			{
				std::cout << "Error: " << res() << std::endl;
			}
		}
		publisher_->delete_contained_entities();
		if (publisher_ != nullptr)
		{
			auto res = participant_->delete_publisher(publisher_);
			if (res != ReturnCode_t::RETCODE_OK)
			{
				std::cout << "Error: " << res() << std::endl;
			}
		}
		if (topic_ != nullptr)
		{
			auto res = participant_->delete_topic(topic_);
			if (res != ReturnCode_t::RETCODE_OK)
			{
				std::cout << "Error: " << res() << std::endl;
			}
		}
		//type_.delete_data(type_.get());
	}

	void setData(void* data) override
	{
		//T* data_p;
		data_ = *(static_cast<T*>(data));
		//std::memcpy(&data_, data, size);
		//data_ = *(data_p);
	}

	void setData() override;

	bool init() override
	{
		type_.register_type(participant_);

		topic_ = participant_->create_topic(config_.topic_name, config_.topic_type_name, eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);
		if (topic_ == nullptr)
		{
			return false;
		}

		// Create the Publisher
		publisher_ = participant_->create_publisher(eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT, nullptr);
		if (publisher_ == nullptr)
		{
			return false;
		}
		std::cout << "Publisher is created" << std::endl;

		eprosima::fastdds::dds::DataWriterQos wqos;
		/*wqos.history().kind = KEEP_LAST_HISTORY_QOS;
		wqos.history().depth = 30;
		wqos.resource_limits().max_samples = 50;
		wqos.resource_limits().allocated_samples = 20;
		wqos.reliable_writer_qos().times.heartbeatPeriod.seconds = 2;
		wqos.reliable_writer_qos().times.heartbeatPeriod.nanosec = 200 * 1000 * 1000;
		wqos.reliability().kind = RELIABLE_RELIABILITY_QOS;*/
		//wqos.deadline().period.nanosec = config_.sleep * 1000 * 1000;

		writer_ = publisher_->create_datawriter(topic_, wqos, &listener_);
		if (writer_ == nullptr)
		{
			return false;
		}

		return true;
	}

	void run() override
	{
		std::cout << "Loop starts with " << config_.sleep << "ms interval" << std::endl;

		while (!stop_ && samples_count_ <= config_.samples)
		{
			if (publish(writer_, &listener_))
			{
				++samples_count_;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config_.sleep));
		}
		//std::this_thread::sleep_for(std::chrono::seconds(20));
		/*if (writer_ != nullptr)
		{
			auto res = publisher_->delete_datawriter(writer_);
			writer_ = nullptr;
			if (res != ReturnCode_t::RETCODE_OK)
			{
				std::cout << "Error: " << res() << std::endl;
			}
		}*/
		
	}

	TopicType getTopicType() override
	{
		return topic_type_;
	}

private:
	// Принимает только данные в этом формате
	T data_;
	const TopicType topic_type_;
	bool stop_;
	size_t samples_count_;

	PublisherConfig config_;

	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Publisher* publisher_;
	eprosima::fastdds::dds::DataWriter* writer_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::TypeSupport type_; // TODO не нужна как поле ?

	class DDSDataListener : public eprosima::fastdds::dds::DataWriterListener
	{
	public:

		DDSDataListener(ConcretePublisher* publisher)
			: matched_(0)
			, first_connected_(false)
			, pub_(publisher)
		{
		}

		~DDSDataListener() override
		{
		}

		void on_publication_matched(
			eprosima::fastdds::dds::DataWriter* writer,
			const eprosima::fastdds::dds::PublicationMatchedStatus& info) override
		{
			if (info.current_count_change == 1)
			{
				matched_ += info.current_count_change;
				std::cout << "ConcretePublisher matched." << std::endl;
				first_connected_ = true;
			}
			else if (info.current_count_change == -1)
			{
				matched_ += info.current_count_change;
				std::cout << "ConcretePublisher unmatched." << std::endl;
				if (matched_ == 0)
				{
					this->pub_->stop_ = true;
				}
			}
			else
			{
				std::cout << info.current_count_change
					<< " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
			}
		}

		ConcretePublisher* pub_;
		int matched_;
		bool first_connected_;

	} listener_;

	bool publish(eprosima::fastdds::dds::DataWriter* writer, const DDSDataListener* listener)
	{
		//std::lock_guard<std::mutex> guard(std::mutex());
		if (!stop_ && listener > 0 && listener->first_connected_)
		{
			specificDataChanging();
			data_.time_service(TimeConverter::GetTime_LLmcs());
			if (!writer->write(&data_))
			{
				return false;
			}
			return true;
		}
		return false;
	}

	void specificDataChanging();
};

class PublisherFactory
{
public:
	virtual ~PublisherFactory() {}
	AbstractDdsPublisher* createPublisher(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const PublisherConfig& config) const;
protected:

};

#endif //!PUBLISHER_FACTORY_H_