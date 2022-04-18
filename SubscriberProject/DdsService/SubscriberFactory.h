#ifndef SUBSCRIBER_FACTORY_H_
#define SUBSCRIBER_FACTORY_H_

//#include <deque>
#include <mutex>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include "../ThreadSafeQueue/ThreadSafeQueue.h"
#include "../../TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "../../BenchmarkTopics/BenchmarkSimplePubSubTypes.h"
#include "../../BenchmarkTopics/BenchmarkVectorPubSubTypes.h"
#include "../../include/TimeConverter/TimeConverter.hpp"
#include "../../include/CommonClasses.h"
#include "../../include/logger/Logger.h"

struct SubscriberConfig
{
	int16_t subscriber_id = 0;
	uint16_t vector_size = 0;
	std::string topic_name = "";
	std::string topic_type_name = "";
	TopicType topic_type = TopicType::UNKNOWN;

	// listener settings
	uint32_t samples = 10;
	uint32_t sleep = 1000;

	friend bool operator==(const SubscriberConfig& lhs, const SubscriberConfig& rhs);
};

class AbstractDdsSubscriber
{
public:
	virtual ~AbstractDdsSubscriber() {};
	virtual bool init() = 0;
	virtual TransitionInfo run() = 0;
	virtual void setConfig(const SubscriberConfig& config) = 0;
	virtual const void* getData() const = 0;
protected:
};

template <class T, class TPubSubType>
class ConcreteSubscriber : public AbstractDdsSubscriber
{
public:
	ConcreteSubscriber(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const SubscriberConfig& config)
		: participant_(participant)
		, subscriber_(nullptr)
		, reader_(nullptr)
		, topic_(nullptr)
		, config_(config)
		, listener_(this)
		, support_type_(new TPubSubType())
		, stop_(false)
	{
		setDataSize();
		log = logger::Logger::getInstance("Subscriber", "logs.txt", true);
	}
	~ConcreteSubscriber() override
	{
		if (reader_ != nullptr)
		{
			auto res = subscriber_->delete_datareader(reader_);
			if (res != ReturnCode_t::RETCODE_OK)
			{
				*log << logger::Logger::e_logType::LOG_ERROR << "Error: " + std::to_string(res());
			}
		}
		subscriber_->delete_contained_entities();
		if (subscriber_ != nullptr)
		{
			auto res = participant_->delete_subscriber(subscriber_);
			if (res != ReturnCode_t::RETCODE_OK)
			{
				*log << logger::Logger::e_logType::LOG_ERROR << "Error: " + std::to_string(res());
			}
		}
		if (topic_ != nullptr)
		{
			auto res = participant_->delete_topic(topic_);
			if (res != ReturnCode_t::RETCODE_OK)
			{
				*log << logger::Logger::e_logType::LOG_ERROR << "Error: " + std::to_string(res());
			}
		}
		*log << logger::Logger::e_logType::LOG_INFO << "Subscriber is deleted";
		//support_type_.delete_data(support_type_.get());
	}

	bool init() override
	{
		*log << logger::Logger::e_logType::LOG_INFO << "Subscriber is creating";
		if (participant_ == nullptr)
		{
			return false;
		}

		support_type_.register_type(participant_);

		topic_ = participant_->create_topic(config_.topic_name, config_.topic_type_name, eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);
		if (topic_ == nullptr)
		{
			return false;
		}

		subscriber_ = participant_->create_subscriber(eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT, nullptr);
		if (subscriber_ == nullptr)
		{
			return false;
		}

		eprosima::fastdds::dds::DataReaderQos rqos;
		/*rqos.history().kind = eprosima::fastdds::dds::KEEP_LAST_HISTORY_QOS;
		rqos.history().depth = 30;
		rqos.resource_limits().max_samples = 50;
		rqos.resource_limits().allocated_samples = 20;
		rqos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;*/
		//rqos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;
		//rqos.deadline().period.nanosec = config_.sleep * 1000 * 1000;

		reader_ = subscriber_->create_datareader(
			topic_,
			rqos,
			&listener_);
		if (reader_ == nullptr)
		{
			return false;
		}
		*log << logger::Logger::e_logType::LOG_INFO << "Subscriber created";
		return true;
	}
	TransitionInfo run() override
	{
		*log << logger::Logger::e_logType::LOG_INFO << "Loop starts with " + std::to_string(config_.sleep) + "ms interval";
		while (!stop_)
		{
			auto kek = eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->lookup_participants(0);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		if (reader_ != nullptr)
		{
			auto res = subscriber_->delete_datareader(reader_);
			reader_ = nullptr;
			if (res != ReturnCode_t::RETCODE_OK)
			{
				*log << logger::Logger::e_logType::LOG_ERROR << "Error: " + std::to_string(res());
			}
		}
		*log << logger::Logger::e_logType::LOG_INFO << "Loop stopped";
		stop_ = false;

		TransitionInfo info;

		if (!data_.empty())
		{
			info.max_delivery_time = data_[0].first.delivery_time;
			info.min_delivery_time = data_[0].first.delivery_time;
			for (const auto& p : data_)
			{
				info.max_delivery_time = info.max_delivery_time > p.first.delivery_time ? 
					info.max_delivery_time : p.first.delivery_time;

				info.min_delivery_time = info.min_delivery_time < p.first.delivery_time ?
					info.min_delivery_time : p.first.delivery_time;
			
				info.delivery_time += p.first.delivery_time;
				info.size += p.first.size;
			}
			*log << logger::Logger::e_logType::LOG_INFO << "Total delivery time:\t" + std::to_string(info.delivery_time);
			*log << logger::Logger::e_logType::LOG_INFO << "Average delivery time:\t" + std::to_string(info.delivery_time / data_.size());
			*log << logger::Logger::e_logType::LOG_INFO << "Max delivery time:\t\t" + std::to_string(info.max_delivery_time);
			*log << logger::Logger::e_logType::LOG_INFO << "Min delivery time:\t\t" + std::to_string(info.min_delivery_time);
		}
		*log << logger::Logger::e_logType::LOG_INFO << "Losted packages:\t\t" + std::to_string(config_.samples - data_.size());
		*log << logger::Logger::e_logType::LOG_INFO << "Total transmitted size:\t" + std::to_string(info.size);
		return info;
	}

	void setConfig(const SubscriberConfig& config) override
	{
		config_ = config;
	}

	const void* getData() const override
	{
		return &data_;
	}

private:
	// Принимает только данные в этом формате
	std::deque<std::pair<PacketInfo, T>> data_;

	T data_sample_;
	
	std::atomic<bool> stop_;

	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Subscriber* subscriber_;
	eprosima::fastdds::dds::DataReader* reader_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::TypeSupport support_type_; // TODO не нужна как поле ?
	SubscriberConfig config_;

	logger::Logger* log;

	void setDataSize() {};

	void runDataSending();

	class SubscriberListener : public eprosima::fastdds::dds::DataReaderListener
	{
	public:
		SubscriberListener(
			ConcreteSubscriber* subscriber)
			: matched_(0)
			, samples_(0)
			, sub_(subscriber)
			, losted_count_(0)
		{
			log = logger::Logger::getInstance("Subscriber", "logs.txt", true);
		}
		~SubscriberListener() override
		{
		}

		// TODO может ли вызываться в разных потоках?
		void on_data_available(
			eprosima::fastdds::dds::DataReader* reader) override
		{
			eprosima::fastdds::dds::SampleInfo info;
			if (reader->take_next_sample(&data_sample_, &info) == ReturnCode_t::RETCODE_OK)
			{
				if (info.valid_data)
				{
					samples_++;
					specificDataHandling(data_sample_);
					//TODO по другому надо как то проверять
					if (samples_ >= sub_->config_.samples)
					{
						sub_->stop_ = true;
					}
				}
			}
		}

		void on_subscription_matched(
			eprosima::fastdds::dds::DataReader* reader,
			const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override
		{
			if (info.current_count_change == 1)
			{
				matched_ += info.current_count_change;
				*log << logger::Logger::e_logType::LOG_INFO << "Subscriber matched";
			}
			else if (info.current_count_change == -1)
			{
				matched_ += info.current_count_change;
				*log << logger::Logger::e_logType::LOG_INFO << "Subscriber unmatched";

				if (info.current_count == 0)
				{
					this->sub_->stop_ = true;
				}
			}
			else
			{
				*log << logger::Logger::e_logType::LOG_INFO << "ConcreteSubscriber: " + std::to_string(info.current_count_change)
					+ " is not a valid value for SubscriptionMatchedStatus current count change";
			}
		}

		/*void on_requested_deadline_missed(
			eprosima::fastdds::dds::DataReader* reader,
			const eprosima::fastrtps::RequestedDeadlineMissedStatus& status) override
		{
			if (status.total_count > 10)
			{
				this->sub_->stop_ = true;
				std::cout << "Missed 10 deadlines" << std::endl;
			}
		}*/

		void specificDataHandling(T& data);

	private:
		std::atomic<int> matched_;
		std::atomic<uint32_t> samples_; // TODO atomic??
		uint32_t losted_count_; // TODO atomic??
		T data_sample_;
		ConcreteSubscriber* sub_;
		logger::Logger* log;
	} listener_;
};

class SubscriberFactory
{
public:
	virtual ~SubscriberFactory() {}
	AbstractDdsSubscriber* createSubscriber(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const SubscriberConfig& config) const;
protected:

};

TopicType string2TopicType(const std::string& type_name);

std::string TopicType2string(TopicType type);

#endif //!SUBSCRIBER_FACTORY_H_