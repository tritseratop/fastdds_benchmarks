#ifndef DDSPUBLISHER_H_
#define DDSPUBLISHER_H_

#include <thread>
#include <mutex>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include "../TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "../BenchmarkTopics/BenchmarkSimplePubSubTypes.h"
#include "../BenchmarkTopics/BenchmarkVectorPubSubTypes.h"
#include "PublisherFactory.h"

class PublisherService
{
public:

    PublisherService(const ServiceConfig<PublisherConfig>& config);
    PublisherService();
    virtual ~PublisherService();

    bool initPublishers();
    eprosima::fastdds::dds::DomainParticipantQos getParticipantQos();
    bool createParticipant();
    void runPublishers();
    void changeSubsConfigAndInit(const ServiceConfig<PublisherConfig>& config);
    void setData();
    void setDdsData(DDSData* data);
    void setDdsDataEx(DDSDataEx* data);
    void setBenchmarkSimple(BenchmarkSimple* data, size_t size);

    bool createNewPublisher(const PublisherConfig& config);
    void deletePublishers();

private:
    ServiceConfig<PublisherConfig> config_;

    PublisherFactory factory_;
    std::vector<AbstractDdsPublisher*> publishers_;

    eprosima::fastdds::dds::DomainParticipant* participant_;

    bool stop;

    void setVectorSizesInDataTopic();
};

#endif /* DDSPUBLISHER_H_ */