#include "DdsPublisher.h"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include "../include/TestUtility.h"

void sendingDdsData(const ServiceConfig<PublisherConfig>& conf)
{
    std::cout << "Sending starts with " << conf.configs.size() << " publisher(s)" << std::endl;
    PublisherService* mypub = new PublisherService(conf);
    if (mypub->initPublishers())
    {
        mypub->runPublishers();
    }
    delete mypub;
}

int main(
    int argc,
    char** argv)
{
    ServiceConfig<PublisherConfig> conf({
        "Participant_pub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1"},
        {}
    });

    if (argc > 1)
    {
        std::string ip = std::string(argv[1]);
        conf.ip = ip;
        //conf.whitelist.push_back(std::string(argv[1]));
    }
    uint32_t samples = 50;
    std::vector<PublisherConfig> confs({
        {0, 10000, "BenchmarkSimple", "BenchmarkSimple", TopicType::BENCHMARK_SIMPLE, samples, 25},
        });
    conf.configs = confs;

    PublisherService* mypub = new PublisherService(conf);
    std::cout << "\nSERVICE RUN WITH TOPIC " << conf.configs[0].topic_name << std::endl;
    std::cout << "Sending starts with " << conf.configs.size() << " publisher(s)" << std::endl;
    if (mypub->initPublishers())
    {
        mypub->runPublishers();
    }

    confs = {
        {0, 10000, "BenchmarkVector", "BenchmarkVector", TopicType::BENCHMARK_VECTOR, samples, 100},
    };
    conf.configs = confs;
    std::cout << "\nSERVICE RUN WITH TOPIC " << conf.configs[0].topic_name << std::endl;
    mypub->changeSubsConfigAndInit(conf);
    mypub->runPublishers();

    conf = {
        "Participant_pub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1"},
        {},
        1000,
        1000,
        1000,
        1000,
        1000,
        1000,
        1000,
        1000,
        1000,
        1000
    };
    confs = {
        {0, 10000, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 300},
    };
    conf.configs = confs;
    std::cout << "\nSERVICE RUN WITH TOPIC " << conf.configs[0].topic_name << std::endl;
    mypub->changeSubsConfigAndInit(conf);
    auto data = getDdsData(750);
    mypub->setDdsData(&data);
    mypub->runPublishers();

    delete mypub;

    system("pause");
}