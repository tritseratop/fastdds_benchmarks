#include "DdsService/DdsSubscriber.h"
#include "../include/TestUtility.h"

void recievingDdsData(const ServiceConfig<SubscriberConfig>& conf)
{
    SubscriberService* mysub = new SubscriberService(conf, nullptr);

    mysub->setVectorSizesInDataTopic();
    if (mysub->initSubscribers())
    {
        mysub->runSubscribers();
    }

    delete mysub;
}

int main(int argc, char* argv[])
{
    uint32_t samples = 50;
    std::vector<SubscriberConfig> configs({
    {0, 10000, "BenchmarkSimple", "BenchmarkSimple", TopicType::BENCHMARK_SIMPLE, samples, 100},
        });

    ServiceConfig<SubscriberConfig> config({
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        configs,
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
        config.ip = std::string(argv[1]);
        //config.whitelist.push_back(std::string(argv[1]));
    }

    //recievingDdsData(config);
    recievingDdsData(config);
    recievingDdsData(config);
    configs = {
    {0, 10000, "BenchmarkSimple1", "BenchmarkSimple", TopicType::BENCHMARK_SIMPLE, samples, 100},
        };
    config.configs = configs;
    recievingDdsData(config);

    std::cout << "4th iteration starts" << std::endl;
    configs = {
    {0, 10000, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100},
    };
    config.configs = configs;
    config.port = 4045;
    recievingDdsData(config);
    system("pause");
}