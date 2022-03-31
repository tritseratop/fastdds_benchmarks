#include "DdsPublisher.h"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include "../include/TestUtility.h"

DDSData getDdsData()
{
    DDSData ddsData;
    ddsData.time_source(0);
    ddsData.time_service(0);
    //std::vector<int> v(10, 1);
    //DataCollectionInt dataCollectionInt;
    //dataCollectionInt.value(v);
    //ddsData.data_int(dataCollectionInt);
    return ddsData;
}

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
        {},
        //1000,
        //1000,
        //1000,
        //1000,
        //1000,
        //1000,
        //1000,
        //1000,
        //1000,
        //1000
    });

    if (argc > 1)
    {
        std::string ip = std::string(argv[1]);
        conf.ip = ip;
        //conf.whitelist.push_back(std::string(argv[1]));
    }
    uint32_t samples = 50;
    std::vector<PublisherConfig> confs({
        {0, 10000, "BenchmarkSimple", "BenchmarkSimple", TopicType::BENCHMARK_SIMPLE, samples, 250},
        });
    conf.configs = confs;
    //std::cout << "Sending starts with 250ms interval" << std::endl;
    //sendingDdsData(conf);


    confs = {
        {0, 10000, "BenchmarkSimple", "BenchmarkSimple", TopicType::BENCHMARK_SIMPLE, samples, 100},
    };
    conf.configs = confs;
    sendingDdsData(conf);

    confs = {
        {0, 10000, "BenchmarkSimple", "BenchmarkSimple", TopicType::BENCHMARK_SIMPLE, samples, 50},
        };
    conf.configs = confs;
    sendingDdsData(conf);

    confs = {
        {0, 10000, "BenchmarkSimple1", "BenchmarkSimple", TopicType::BENCHMARK_SIMPLE, samples, 25},
        };
    conf.configs = confs;
    sendingDdsData(conf);

    std::cout << "4th iteration starts" << std::endl;
    confs = {
        {0, 10000, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 25},
    };
    conf.configs = confs;
    conf.port = 4045;
    sendingDdsData(conf);

    system("pause");
}