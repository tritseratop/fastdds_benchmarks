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
#include "../../include/TimeConverter/TimeConverter.hpp"
#include "../../include/CommonClasses.h"

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
	}
	~ConcreteSubscriber() override
	{
		if (reader_ != nullptr)
		{
			auto res = subscriber_->delete_datareader(reader_);
			if (res != ReturnCode_t::RETCODE_OK)
			{
				std::cout << "Error: " << res() << std::endl;
			}
		}
		subscriber_->delete_contained_entities();
		if (subscriber_ != nullptr)
		{
			auto res = participant_->delete_subscriber(subscriber_);
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
		std::cout << "Subscriber is deleted" << std::endl;
		//support_type_.delete_data(support_type_.get());
	}

	bool init() override
	{
		std::cout << "Subscriber is creating" << std::endl;
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

		reader_ = subscriber_->create_datareader(
			topic_,
			eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT,
			&listener_);
		if (reader_ == nullptr)
		{
			return false;
		}
		std::cout << "Subscriber created" << std::endl;
		return true;
	}
	TransitionInfo run() override
	{
		std::cout << "Loop starts with " << config_.sleep << "ms interval" << std::endl;
		while (!stop_)
		{
			auto kek = eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->lookup_participants(0);
			std::this_thread::sleep_for(std::chrono::milliseconds(config_.sleep));
		}
		TransitionInfo info;
		for (const auto& p : data_)
		{
			info.delivery_time += p.first.delivery_time;
			info.size += p.first.size;
		}
		std::cout << "Total delivery time: " << info.delivery_time << std::endl;
		if (!data_.empty())
		{
			std::cout << "Average delivery time: " << info.delivery_time / data_.size() << std::endl;
		}
		std::cout << "Total transmitted size: " << info.size << std::endl;
		std::cout << "Number of losted packeges: " << listener_.losted_count_ << std::endl;
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
	// ��������� ������ ������ � ���� �������
	std::deque<std::pair<PacketInfo, T>> data_;

	T data_sample_;
	
	bool stop_;

	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Subscriber* subscriber_;
	eprosima::fastdds::dds::DataReader* reader_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::TypeSupport support_type_; // TODO �� ����� ��� ���� ?
	SubscriberConfig config_;

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
		}
		~SubscriberListener() override
		{
		}

		// TODO ����� �� ���������� � ������ �������?
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
					//TODO �� ������� ���� ��� �� ���������
					{
						if (samples_ >= sub_->config_.samples)
						{
							sub_->stop_ = true;
						}
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
				std::cout << "ConcreteSubscriber matched." << std::endl;
			}
			else if (info.current_count_change == -1)
			{
				matched_ += info.current_count_change;
				std::cout << "ConcreteSubscriber unmatched." << std::endl;

				if (matched_ == 0)
				{
					this->sub_->stop_ = true;
				}
			}
			else
			{
				std::cout << "ConcreteSubscriber: " << info.current_count_change
					<< " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
			}
		}

		void specificDataHandling(T& data);

		int matched_;
		uint32_t samples_; // TODO atomic??
		uint32_t losted_count_; // TODO atomic??
		T data_sample_;
		ConcreteSubscriber* sub_;
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