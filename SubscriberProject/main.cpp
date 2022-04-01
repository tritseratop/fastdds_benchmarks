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
        configs
        });

    if (argc > 1)
    {
        config.ip = std::string(argv[1]);
    }

    config.configs = configs;
    SubscriberService* mysub = new SubscriberService(config, nullptr);

    mysub->setVectorSizesInDataTopic();
    std::cout << "\nSERVICE RUN WITH TOPIC " << config.configs[0].topic_type_name << std::endl;
    if (mysub->initSubscribers())
    {
        mysub->runSubscribers();
    }

    configs = {
        {0, 10000, "BenchmarkVector", "BenchmarkVector", TopicType::BENCHMARK_VECTOR, samples, 100},
    };
    config.configs = configs;
    std::cout << "\nSERVICE RUN WITH TOPIC " << config.configs[0].topic_type_name << std::endl;
    mysub->changeSubsConfig(config);
    mysub->runSubscribers();

    config = {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        configs,
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

    configs = {
        {0, 10000, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 300},
    };
    config.configs = configs;
    std::cout << "\nSERVICE RUN WITH TOPIC " << config.configs[0].topic_type_name << std::endl;
    mysub->changeSubsConfig(config);
    mysub->runSubscribers();

    delete mysub;
    system("pause");
}