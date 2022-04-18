#include "DdsPublisher.h"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

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
    std::vector<uint16_t> sizes = {10, 100, 1000, 10000};
    std::vector<uint32_t> v_sleep = {100};
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
    ServiceConfig<PublisherConfig> default_service_config = {
        "Participant_pub",
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
    PublisherConfig ddsdata_config = {
        0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 1000
    };
    PublisherConfig ddsdataex_config = {
        0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 1000
    };
    std::vector<ServiceConfig<PublisherConfig>> configs;
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

    PublisherService* mypub = new PublisherService();
    int i = 1;
    for (auto conf : configs)
    {
        std::cout << "\n" << i++ << ". SERVICE RUN WITH TOPIC " << conf.configs[0].topic_name 
            << " size: " << conf.configs[0].vector_size << std::endl;
        mypub->changeSubsConfigAndInit(conf);
        mypub->setData();
        mypub->runPublishers();
    }

    delete mypub;

    system("pause");
}