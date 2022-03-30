#include "CommonClasses.h"

TopicType string2TopicType(std::string type_name)
{
	if (type_name == "DDSData")
	{
		return TopicType::DDS_DATA;
	}
	else if (type_name == "DDSDataEx")
	{
		return TopicType::DDS_DATA_EX;
	}
	else if (type_name == "DDSAlarm")
	{
		return TopicType::DDS_ALARM;
	}
	else if (type_name == "DDSAlarmEx")
	{
		return TopicType::DDS_EX_ALARM;
	}
	else if (type_name == "BenchmarkSimple")
	{
		return TopicType::BENCHMARK_SIMPLE;
	}
	else
	{
		return TopicType::UNKNOWN;
	}
}

std::string TopicType2string(TopicType type)
{
	switch (type)
	{
	case TopicType::DDS_DATA:
		return "DDSData";
	case TopicType::DDS_DATA_EX:
		return "DDSDataEx";
	case TopicType::DDS_ALARM:
		return "DDSAlarm";
	case TopicType::DDS_EX_ALARM:
		return "DDSAlarmEx";
	case TopicType::BENCHMARK_SIMPLE:
		return "BenchmarkSimple";
	default:
		return "";
	}
}