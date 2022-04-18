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
    std::vector<uint16_t> sizes = { 10, 100, 1000, 10000 };
    std::vector<uint32_t> v_sleep = { 100 };
    uint32_t samples = 50;
    std::string ip = "127.0.0.1";
    Transport transport = Transport::TCP;

    if (argc > 1)
    {
        ip = std::string(argv[1]);
        if (argc > 2)
        {
            transport = Transport::UDP;
        }
    }
    ServiceConfig<SubscriberConfig> default_service_config = {
        "Participant_sub",
        transport,
        ip,
        1111,
        {"127.0.0.1"},
        {},
        10000,
        10000,
        10000,
        10000,
        10000,
        10000,
        10000,
        10000,
        10000,
        10000
    };
    SubscriberConfig ddsdata_config = {
        0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100
    };
    SubscriberConfig ddsdataex_config = {
        0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100
    };
    std::vector<ServiceConfig<SubscriberConfig>> configs;
    for (auto size : sizes)
    {
        ddsdata_config.vector_size = size;
        ddsdataex_config.vector_size = size;
        for (auto sleep : v_sleep)
        {
            ddsdata_config.sleep = sleep;
            default_service_config.configs = { ddsdata_config };
            configs.push_back(default_service_config);

            ddsdataex_config.sleep = sleep;
            default_service_config.configs = { ddsdataex_config };
            configs.push_back(default_service_config);

        }
    }

    SubscriberService* mysub = new SubscriberService();

    int i = 1;
    for (auto conf : configs)
    {
        std::cout << "\n\n" << i++ << ". SERVICE RUN WITH TOPIC " << conf.configs[0].topic_type_name 
            << " size: " << conf.configs[0].vector_size << std::endl;
        mysub->changeSubsConfigAndInit(conf);
        mysub->runSubscribers();
    }
    delete mysub;

    system("pause");
}