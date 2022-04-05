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

    std::vector<ServiceConfig<SubscriberConfig>> configs = {
        {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        {
            {0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100},
        },
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
        },
        {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        {
            {0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100},
        },
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
        },
        {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        {
            {0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100},
        },
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
        },
        {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        {
            {0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100},
        },
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
        },
        {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        {
            {0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100},
        },
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
        },
        {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        {
            {0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100},
        },
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
        },
    };

    if (argc > 1)
    {
        std::string ip = std::string(argv[1]);
        for (auto conf : configs)
        {
            conf.ip = ip;
        }
    }

    SubscriberService* mysub = new SubscriberService();

    for (auto conf : configs)
    {
        std::cout << "\nSERVICE RUN WITH TOPIC " << conf.configs[0].topic_type_name << std::endl;
        mysub->changeSubsConfigAndInit(conf);
        mysub->runSubscribers();
    }

    delete mysub;

    std::vector<ServiceConfig<SubscriberConfig>> configs1 = {
        {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        {
            {0, 100, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100},
        },
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
        },
        {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        {
            {0, 100, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100},
        },
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
        },
        {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        {
            {0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100},
        },
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
        },
        {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        {
            {0, 100, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100},
        },
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
        },
        {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        {
            {0, 100, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100},
        },
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
        },
        {
        "Participant_sub",
        Transport::TCP,
        "127.0.0.1",
        1111,
        {"127.0.0.1", "192.168.0.185", "192.168.0.186"},
        {
            {0, 100, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100},
        },
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
        },
    };

    SubscriberService* mysub1 = new SubscriberService();

    for (auto conf : configs1)
    {
        std::cout << "\nSERVICE RUN WITH TOPIC " << conf.configs[0].topic_type_name << std::endl;
        mysub1->changeSubsConfigAndInit(conf);
        mysub1->runSubscribers();
    }

    delete mysub1;

    system("pause");
}