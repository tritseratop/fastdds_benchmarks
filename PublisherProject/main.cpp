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
    uint32_t samples = 50;
    std::vector< ServiceConfig<PublisherConfig> > configs =
    {
        {
            "Participant_pub",
            Transport::TCP,
            "127.0.0.1",
            1111,
            {"127.0.0.1"},
            {
                {0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100},
            },
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10
        },
        {
            "Participant_pub",
            Transport::TCP,
            "127.0.0.1",
            1111,
            {"127.0.0.1"},
            {
                {0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100},
            },
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10
        },
        {
            "Participant_pub",
            Transport::TCP,
            "127.0.0.1",
            1111,
            {"127.0.0.1"},
            {
                {0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 500},
            },
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10
        },
        {
            "Participant_pub",
            Transport::TCP,
            "127.0.0.1",
            1111,
            {"127.0.0.1"},
            {
                {0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 500},
            },
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10
        },
        {
            "Participant_pub",
            Transport::TCP,
            "127.0.0.1",
            1111,
            {"127.0.0.1"},
            {
                {0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100},
            },
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10
        },
        {
            "Participant_pub",
            Transport::TCP,
            "127.0.0.1",
            1111,
            {"127.0.0.1"},
            {
                {0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100},
            },
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10,
            10
        }
    };

    if (argc > 1)
    {
        std::string ip = std::string(argv[1]);        
        for (auto conf : configs)
        {
            conf.ip = ip;
        }
    }

    PublisherService* mypub = new PublisherService();
    for (auto conf : configs)
    {
        std::cout << "\nSERVICE RUN WITH TOPIC " << conf.configs[0].topic_name << std::endl;
        mypub->changeSubsConfigAndInit(conf);
        mypub->setData();
        mypub->runPublishers();
    }

    delete mypub;

    std::vector< ServiceConfig<PublisherConfig> > configs1 =
    {
        {
            "Participant_pub",
            Transport::TCP,
            "127.0.0.1",
            1111,
            {"127.0.0.1"},
            {
                {0, 100, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100},
            },
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
        },
        {
            "Participant_pub",
            Transport::TCP,
            "127.0.0.1",
            1111,
            {"127.0.0.1"},
            {
                {0, 100, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100},
            },
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
        },
        {
            "Participant_pub",
            Transport::TCP,
            "127.0.0.1",
            1111,
            {"127.0.0.1"},
            {
                {0, 100, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 500},
            },
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
        },
        {
            "Participant_pub",
            Transport::TCP,
            "127.0.0.1",
            1111,
            {"127.0.0.1"},
            {
                {0, 100, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 500},
            },
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
        },
        {
            "Participant_pub",
            Transport::TCP,
            "127.0.0.1",
            1111,
            {"127.0.0.1"},
            {
                {0, 100, "DDSData", "DDSData", TopicType::DDS_DATA, samples, 100},
            },
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
        },
        {
            "Participant_pub",
            Transport::TCP,
            "127.0.0.1",
            1111,
            {"127.0.0.1"},
            {
                {0, 100, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, samples, 100},
            },
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
        }
    };

    /*PublisherService* mypub1 = new PublisherService();
    for (auto conf : configs1)
    {
        std::cout << "\nSERVICE RUN WITH TOPIC " << conf.configs[0].topic_name << std::endl;
        mypub1->changeSubsConfigAndInit(conf);
        mypub1->setData();
        mypub1->runPublishers();
    }

    delete mypub1;*/

    system("pause");
}