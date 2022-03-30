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
#include "PublisherFactory.h"

class PublisherService
{
public:

    PublisherService(const ServiceConfig<PublisherConfig>& config);
    virtual ~PublisherService();

    bool initPublishers();
    void runPublishers();
    void changeSubsConfig(const ServiceConfig<PublisherConfig>& config);

    void setDdsData(DDSData* data, size_t size);
    void setDdsDataEx(DDSDataEx* data, size_t size);
    void setBenchmarkSimple(BenchmarkSimple* data, size_t size);

    bool createNewPublisher(const PublisherConfig& config);

private:
    ServiceConfig<PublisherConfig> config_;

    PublisherFactory factory_;
    std::vector<AbstractDdsPublisher*> publishers_;

    eprosima::fastdds::dds::DomainParticipant* participant_;

    bool stop;

    void deletePublishers();
    void setVectorSizesInDataTopic();
};

#endif /* DDSPUBLISHER_H_ */