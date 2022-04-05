// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file DDSAlarmExTypeObject.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace { char dummy; }
#endif

#include "DDSAlarmEx.h"
#include "DDSAlarmExTypeObject.h"
#include <utility>
#include <sstream>
#include <fastrtps/rtps/common/SerializedPayload.h>
#include <fastrtps/utils/md5.h>
#include <fastrtps/types/TypeObjectFactory.h>
#include <fastrtps/types/TypeNamesGenerator.h>
#include <fastrtps/types/AnnotationParameterValue.h>
#include <fastcdr/FastBuffer.h>
#include <fastcdr/Cdr.h>

using namespace eprosima::fastrtps::rtps;

void registerDDSAlarmExTypes()
{
    TypeObjectFactory *factory = TypeObjectFactory::get_instance();
    factory->add_type_object("Alarm", GetAlarmIdentifier(true), GetAlarmObject(true));
    factory->add_type_object("Alarm", GetAlarmIdentifier(false), GetAlarmObject(false));

    factory->add_type_object("DDSAlarmEx", GetDDSAlarmExIdentifier(true), GetDDSAlarmExObject(true));
    factory->add_type_object("DDSAlarmEx", GetDDSAlarmExIdentifier(false), GetDDSAlarmExObject(false));

}

const TypeIdentifier* GetAlarmIdentifier(bool complete)
{
    const TypeIdentifier * c_identifier = TypeObjectFactory::get_instance()->get_type_identifier("Alarm", complete);
    if (c_identifier != nullptr && (!complete || c_identifier->_d() == EK_COMPLETE))
    {
        return c_identifier;
    }

    GetAlarmObject(complete); // Generated inside
    return TypeObjectFactory::get_instance()->get_type_identifier("Alarm", complete);
}

const TypeObject* GetAlarmObject(bool complete)
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("Alarm", complete);
    if (c_type_object != nullptr)
    {
        return c_type_object;
    }
    else if (complete)
    {
        return GetCompleteAlarmObject();
    }
    //else
    return GetMinimalAlarmObject();
}

const TypeObject* GetMinimalAlarmObject()
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("Alarm", false);
    if (c_type_object != nullptr)
    {
        return c_type_object;
    }

    TypeObject *type_object = new TypeObject();
    type_object->_d(EK_MINIMAL);
    type_object->minimal()._d(TK_STRUCTURE);

    type_object->minimal().struct_type().struct_flags().IS_FINAL(false);
    type_object->minimal().struct_type().struct_flags().IS_APPENDABLE(false);
    type_object->minimal().struct_type().struct_flags().IS_MUTABLE(false);
    type_object->minimal().struct_type().struct_flags().IS_NESTED(false);
    type_object->minimal().struct_type().struct_flags().IS_AUTOID_HASH(false); // Unsupported

    MemberId memberId = 0;
    MinimalStructMember mst_time_source;
    mst_time_source.common().member_id(memberId++);
    mst_time_source.common().member_flags().TRY_CONSTRUCT1(false); // Unsupported
    mst_time_source.common().member_flags().TRY_CONSTRUCT2(false); // Unsupported
    mst_time_source.common().member_flags().IS_EXTERNAL(false); // Unsupported
    mst_time_source.common().member_flags().IS_OPTIONAL(false);
    mst_time_source.common().member_flags().IS_MUST_UNDERSTAND(false);
    mst_time_source.common().member_flags().IS_KEY(false);
    mst_time_source.common().member_flags().IS_DEFAULT(false); // Doesn't apply
    {
        std::string cppType = "int64_t";
        if (cppType == "long double")
        {
            cppType = "longdouble";
        }
        mst_time_source.common().member_type_id(*TypeObjectFactory::get_instance()->get_type_identifier(cppType, false));
    }

    MD5 time_source_hash("time_source");
    for(int i = 0; i < 4; ++i)
    {
        mst_time_source.detail().name_hash()[i] = time_source_hash.digest[i];
    }
    type_object->minimal().struct_type().member_seq().emplace_back(mst_time_source);

    MinimalStructMember mst_id_tag;
    mst_id_tag.common().member_id(memberId++);
    mst_id_tag.common().member_flags().TRY_CONSTRUCT1(false); // Unsupported
    mst_id_tag.common().member_flags().TRY_CONSTRUCT2(false); // Unsupported
    mst_id_tag.common().member_flags().IS_EXTERNAL(false); // Unsupported
    mst_id_tag.common().member_flags().IS_OPTIONAL(false);
    mst_id_tag.common().member_flags().IS_MUST_UNDERSTAND(false);
    mst_id_tag.common().member_flags().IS_KEY(false);
    mst_id_tag.common().member_flags().IS_DEFAULT(false); // Doesn't apply
    {
        std::string cppType = "uint32_t";
        if (cppType == "long double")
        {
            cppType = "longdouble";
        }
        mst_id_tag.common().member_type_id(*TypeObjectFactory::get_instance()->get_type_identifier(cppType, false));
    }

    MD5 id_tag_hash("id_tag");
    for(int i = 0; i < 4; ++i)
    {
        mst_id_tag.detail().name_hash()[i] = id_tag_hash.digest[i];
    }
    type_object->minimal().struct_type().member_seq().emplace_back(mst_id_tag);

    MinimalStructMember mst_value;
    mst_value.common().member_id(memberId++);
    mst_value.common().member_flags().TRY_CONSTRUCT1(false); // Unsupported
    mst_value.common().member_flags().TRY_CONSTRUCT2(false); // Unsupported
    mst_value.common().member_flags().IS_EXTERNAL(false); // Unsupported
    mst_value.common().member_flags().IS_OPTIONAL(false);
    mst_value.common().member_flags().IS_MUST_UNDERSTAND(false);
    mst_value.common().member_flags().IS_KEY(false);
    mst_value.common().member_flags().IS_DEFAULT(false); // Doesn't apply
    {
        std::string cppType = "char";
        if (cppType == "long double")
        {
            cppType = "longdouble";
        }
        mst_value.common().member_type_id(*TypeObjectFactory::get_instance()->get_type_identifier(cppType, false));
    }

    MD5 value_hash("value");
    for(int i = 0; i < 4; ++i)
    {
        mst_value.detail().name_hash()[i] = value_hash.digest[i];
    }
    type_object->minimal().struct_type().member_seq().emplace_back(mst_value);

    MinimalStructMember mst_quality;
    mst_quality.common().member_id(memberId++);
    mst_quality.common().member_flags().TRY_CONSTRUCT1(false); // Unsupported
    mst_quality.common().member_flags().TRY_CONSTRUCT2(false); // Unsupported
    mst_quality.common().member_flags().IS_EXTERNAL(false); // Unsupported
    mst_quality.common().member_flags().IS_OPTIONAL(false);
    mst_quality.common().member_flags().IS_MUST_UNDERSTAND(false);
    mst_quality.common().member_flags().IS_KEY(false);
    mst_quality.common().member_flags().IS_DEFAULT(false); // Doesn't apply
    {
        std::string cppType = "char";
        if (cppType == "long double")
        {
            cppType = "longdouble";
        }
        mst_quality.common().member_type_id(*TypeObjectFactory::get_instance()->get_type_identifier(cppType, false));
    }

    MD5 quality_hash("quality");
    for(int i = 0; i < 4; ++i)
    {
        mst_quality.detail().name_hash()[i] = quality_hash.digest[i];
    }
    type_object->minimal().struct_type().member_seq().emplace_back(mst_quality);


    // Header
    // TODO Inheritance
    //type_object->minimal().struct_type().header().base_type()._d(EK_MINIMAL);
    //type_object->minimal().struct_type().header().base_type().equivalence_hash()[0..13];

    TypeIdentifier identifier;
    identifier._d(EK_MINIMAL);

    SerializedPayload_t payload(static_cast<uint32_t>(
        MinimalStructType::getCdrSerializedSize(type_object->minimal().struct_type()) + 4));
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload.data, payload.max_size);
    // Fixed endian (Page 221, EquivalenceHash definition of Extensible and Dynamic Topic Types for DDS document)
    eprosima::fastcdr::Cdr ser(
        fastbuffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS,
        eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload.encapsulation = CDR_LE;

    type_object->serialize(ser);
    payload.length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    MD5 objectHash;
    objectHash.update((char*)payload.data, payload.length);
    objectHash.finalize();
    for(int i = 0; i < 14; ++i)
    {
        identifier.equivalence_hash()[i] = objectHash.digest[i];
    }

    TypeObjectFactory::get_instance()->add_type_object("Alarm", &identifier, type_object);
    delete type_object;
    return TypeObjectFactory::get_instance()->get_type_object("Alarm", false);
}

const TypeObject* GetCompleteAlarmObject()
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("Alarm", true);
    if (c_type_object != nullptr && c_type_object->_d() == EK_COMPLETE)
    {
        return c_type_object;
    }

    TypeObject *type_object = new TypeObject();
    type_object->_d(EK_COMPLETE);
    type_object->complete()._d(TK_STRUCTURE);

    type_object->complete().struct_type().struct_flags().IS_FINAL(false);
    type_object->complete().struct_type().struct_flags().IS_APPENDABLE(false);
    type_object->complete().struct_type().struct_flags().IS_MUTABLE(false);
    type_object->complete().struct_type().struct_flags().IS_NESTED(false);
    type_object->complete().struct_type().struct_flags().IS_AUTOID_HASH(false); // Unsupported

    MemberId memberId = 0;
    CompleteStructMember cst_time_source;
    cst_time_source.common().member_id(memberId++);
    cst_time_source.common().member_flags().TRY_CONSTRUCT1(false); // Unsupported
    cst_time_source.common().member_flags().TRY_CONSTRUCT2(false); // Unsupported
    cst_time_source.common().member_flags().IS_EXTERNAL(false); // Unsupported
    cst_time_source.common().member_flags().IS_OPTIONAL(false);
    cst_time_source.common().member_flags().IS_MUST_UNDERSTAND(false);
    cst_time_source.common().member_flags().IS_KEY(false);
    cst_time_source.common().member_flags().IS_DEFAULT(false); // Doesn't apply
    {
        std::string cppType = "int64_t";
        if (cppType == "long double")
        {
            cppType = "longdouble";
        }
        cst_time_source.common().member_type_id(*TypeObjectFactory::get_instance()->get_type_identifier(cppType, false));
    }

    cst_time_source.detail().name("time_source");

    type_object->complete().struct_type().member_seq().emplace_back(cst_time_source);

    CompleteStructMember cst_id_tag;
    cst_id_tag.common().member_id(memberId++);
    cst_id_tag.common().member_flags().TRY_CONSTRUCT1(false); // Unsupported
    cst_id_tag.common().member_flags().TRY_CONSTRUCT2(false); // Unsupported
    cst_id_tag.common().member_flags().IS_EXTERNAL(false); // Unsupported
    cst_id_tag.common().member_flags().IS_OPTIONAL(false);
    cst_id_tag.common().member_flags().IS_MUST_UNDERSTAND(false);
    cst_id_tag.common().member_flags().IS_KEY(false);
    cst_id_tag.common().member_flags().IS_DEFAULT(false); // Doesn't apply
    {
        std::string cppType = "uint32_t";
        if (cppType == "long double")
        {
            cppType = "longdouble";
        }
        cst_id_tag.common().member_type_id(*TypeObjectFactory::get_instance()->get_type_identifier(cppType, false));
    }

    cst_id_tag.detail().name("id_tag");

    type_object->complete().struct_type().member_seq().emplace_back(cst_id_tag);

    CompleteStructMember cst_value;
    cst_value.common().member_id(memberId++);
    cst_value.common().member_flags().TRY_CONSTRUCT1(false); // Unsupported
    cst_value.common().member_flags().TRY_CONSTRUCT2(false); // Unsupported
    cst_value.common().member_flags().IS_EXTERNAL(false); // Unsupported
    cst_value.common().member_flags().IS_OPTIONAL(false);
    cst_value.common().member_flags().IS_MUST_UNDERSTAND(false);
    cst_value.common().member_flags().IS_KEY(false);
    cst_value.common().member_flags().IS_DEFAULT(false); // Doesn't apply
    {
        std::string cppType = "char";
        if (cppType == "long double")
        {
            cppType = "longdouble";
        }
        cst_value.common().member_type_id(*TypeObjectFactory::get_instance()->get_type_identifier(cppType, false));
    }

    cst_value.detail().name("value");

    type_object->complete().struct_type().member_seq().emplace_back(cst_value);

    CompleteStructMember cst_quality;
    cst_quality.common().member_id(memberId++);
    cst_quality.common().member_flags().TRY_CONSTRUCT1(false); // Unsupported
    cst_quality.common().member_flags().TRY_CONSTRUCT2(false); // Unsupported
    cst_quality.common().member_flags().IS_EXTERNAL(false); // Unsupported
    cst_quality.common().member_flags().IS_OPTIONAL(false);
    cst_quality.common().member_flags().IS_MUST_UNDERSTAND(false);
    cst_quality.common().member_flags().IS_KEY(false);
    cst_quality.common().member_flags().IS_DEFAULT(false); // Doesn't apply
    {
        std::string cppType = "char";
        if (cppType == "long double")
        {
            cppType = "longdouble";
        }
        cst_quality.common().member_type_id(*TypeObjectFactory::get_instance()->get_type_identifier(cppType, false));
    }

    cst_quality.detail().name("quality");

    type_object->complete().struct_type().member_seq().emplace_back(cst_quality);


    // Header
    type_object->complete().struct_type().header().detail().type_name("Alarm");
    // TODO inheritance


    TypeIdentifier identifier;
    identifier._d(EK_COMPLETE);

    SerializedPayload_t payload(static_cast<uint32_t>(
        CompleteStructType::getCdrSerializedSize(type_object->complete().struct_type()) + 4));
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload.data, payload.max_size);
    // Fixed endian (Page 221, EquivalenceHash definition of Extensible and Dynamic Topic Types for DDS document)
    eprosima::fastcdr::Cdr ser(
        fastbuffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS,
        eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload.encapsulation = CDR_LE;

    type_object->serialize(ser);
    payload.length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    MD5 objectHash;
    objectHash.update((char*)payload.data, payload.length);
    objectHash.finalize();
    for(int i = 0; i < 14; ++i)
    {
        identifier.equivalence_hash()[i] = objectHash.digest[i];
    }

    TypeObjectFactory::get_instance()->add_type_object("Alarm", &identifier, type_object);
    delete type_object;
    return TypeObjectFactory::get_instance()->get_type_object("Alarm", true);
}

const TypeIdentifier* GetDDSAlarmExIdentifier(bool complete)
{
    const TypeIdentifier * c_identifier = TypeObjectFactory::get_instance()->get_type_identifier("DDSAlarmEx", complete);
    if (c_identifier != nullptr && (!complete || c_identifier->_d() == EK_COMPLETE))
    {
        return c_identifier;
    }

    GetDDSAlarmExObject(complete); // Generated inside
    return TypeObjectFactory::get_instance()->get_type_identifier("DDSAlarmEx", complete);
}

const TypeObject* GetDDSAlarmExObject(bool complete)
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("DDSAlarmEx", complete);
    if (c_type_object != nullptr)
    {
        return c_type_object;
    }
    else if (complete)
    {
        return GetCompleteDDSAlarmExObject();
    }
    //else
    return GetMinimalDDSAlarmExObject();
}

const TypeObject* GetMinimalDDSAlarmExObject()
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("DDSAlarmEx", false);
    if (c_type_object != nullptr)
    {
        return c_type_object;
    }

    TypeObject *type_object = new TypeObject();
    type_object->_d(EK_MINIMAL);
    type_object->minimal()._d(TK_STRUCTURE);

    type_object->minimal().struct_type().struct_flags().IS_FINAL(false);
    type_object->minimal().struct_type().struct_flags().IS_APPENDABLE(false);
    type_object->minimal().struct_type().struct_flags().IS_MUTABLE(false);
    type_object->minimal().struct_type().struct_flags().IS_NESTED(false);
    type_object->minimal().struct_type().struct_flags().IS_AUTOID_HASH(false); // Unsupported

    MemberId memberId = 0;
    MinimalStructMember mst_time_service;
    mst_time_service.common().member_id(memberId++);
    mst_time_service.common().member_flags().TRY_CONSTRUCT1(false); // Unsupported
    mst_time_service.common().member_flags().TRY_CONSTRUCT2(false); // Unsupported
    mst_time_service.common().member_flags().IS_EXTERNAL(false); // Unsupported
    mst_time_service.common().member_flags().IS_OPTIONAL(false);
    mst_time_service.common().member_flags().IS_MUST_UNDERSTAND(false);
    mst_time_service.common().member_flags().IS_KEY(false);
    mst_time_service.common().member_flags().IS_DEFAULT(false); // Doesn't apply
    {
        std::string cppType = "int64_t";
        if (cppType == "long double")
        {
            cppType = "longdouble";
        }
        mst_time_service.common().member_type_id(*TypeObjectFactory::get_instance()->get_type_identifier(cppType, false));
    }

    MD5 time_service_hash("time_service");
    for(int i = 0; i < 4; ++i)
    {
        mst_time_service.detail().name_hash()[i] = time_service_hash.digest[i];
    }
    type_object->minimal().struct_type().member_seq().emplace_back(mst_time_service);

    MinimalStructMember mst_alarms;
    mst_alarms.common().member_id(memberId++);
    mst_alarms.common().member_flags().TRY_CONSTRUCT1(false); // Unsupported
    mst_alarms.common().member_flags().TRY_CONSTRUCT2(false); // Unsupported
    mst_alarms.common().member_flags().IS_EXTERNAL(false); // Unsupported
    mst_alarms.common().member_flags().IS_OPTIONAL(false);
    mst_alarms.common().member_flags().IS_MUST_UNDERSTAND(false);
    mst_alarms.common().member_flags().IS_KEY(false);
    mst_alarms.common().member_flags().IS_DEFAULT(false); // Doesn't apply
    mst_alarms.common().member_type_id(*TypeObjectFactory::get_instance()->get_sequence_identifier("Alarm", 0, false));


    MD5 alarms_hash("alarms");
    for(int i = 0; i < 4; ++i)
    {
        mst_alarms.detail().name_hash()[i] = alarms_hash.digest[i];
    }
    type_object->minimal().struct_type().member_seq().emplace_back(mst_alarms);


    // Header
    // TODO Inheritance
    //type_object->minimal().struct_type().header().base_type()._d(EK_MINIMAL);
    //type_object->minimal().struct_type().header().base_type().equivalence_hash()[0..13];

    TypeIdentifier identifier;
    identifier._d(EK_MINIMAL);

    SerializedPayload_t payload(static_cast<uint32_t>(
        MinimalStructType::getCdrSerializedSize(type_object->minimal().struct_type()) + 4));
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload.data, payload.max_size);
    // Fixed endian (Page 221, EquivalenceHash definition of Extensible and Dynamic Topic Types for DDS document)
    eprosima::fastcdr::Cdr ser(
        fastbuffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS,
        eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload.encapsulation = CDR_LE;

    type_object->serialize(ser);
    payload.length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    MD5 objectHash;
    objectHash.update((char*)payload.data, payload.length);
    objectHash.finalize();
    for(int i = 0; i < 14; ++i)
    {
        identifier.equivalence_hash()[i] = objectHash.digest[i];
    }

    TypeObjectFactory::get_instance()->add_type_object("DDSAlarmEx", &identifier, type_object);
    delete type_object;
    return TypeObjectFactory::get_instance()->get_type_object("DDSAlarmEx", false);
}

const TypeObject* GetCompleteDDSAlarmExObject()
{
    const TypeObject* c_type_object = TypeObjectFactory::get_instance()->get_type_object("DDSAlarmEx", true);
    if (c_type_object != nullptr && c_type_object->_d() == EK_COMPLETE)
    {
        return c_type_object;
    }

    TypeObject *type_object = new TypeObject();
    type_object->_d(EK_COMPLETE);
    type_object->complete()._d(TK_STRUCTURE);

    type_object->complete().struct_type().struct_flags().IS_FINAL(false);
    type_object->complete().struct_type().struct_flags().IS_APPENDABLE(false);
    type_object->complete().struct_type().struct_flags().IS_MUTABLE(false);
    type_object->complete().struct_type().struct_flags().IS_NESTED(false);
    type_object->complete().struct_type().struct_flags().IS_AUTOID_HASH(false); // Unsupported

    MemberId memberId = 0;
    CompleteStructMember cst_time_service;
    cst_time_service.common().member_id(memberId++);
    cst_time_service.common().member_flags().TRY_CONSTRUCT1(false); // Unsupported
    cst_time_service.common().member_flags().TRY_CONSTRUCT2(false); // Unsupported
    cst_time_service.common().member_flags().IS_EXTERNAL(false); // Unsupported
    cst_time_service.common().member_flags().IS_OPTIONAL(false);
    cst_time_service.common().member_flags().IS_MUST_UNDERSTAND(false);
    cst_time_service.common().member_flags().IS_KEY(false);
    cst_time_service.common().member_flags().IS_DEFAULT(false); // Doesn't apply
    {
        std::string cppType = "int64_t";
        if (cppType == "long double")
        {
            cppType = "longdouble";
        }
        cst_time_service.common().member_type_id(*TypeObjectFactory::get_instance()->get_type_identifier(cppType, false));
    }

    cst_time_service.detail().name("time_service");

    type_object->complete().struct_type().member_seq().emplace_back(cst_time_service);

    CompleteStructMember cst_alarms;
    cst_alarms.common().member_id(memberId++);
    cst_alarms.common().member_flags().TRY_CONSTRUCT1(false); // Unsupported
    cst_alarms.common().member_flags().TRY_CONSTRUCT2(false); // Unsupported
    cst_alarms.common().member_flags().IS_EXTERNAL(false); // Unsupported
    cst_alarms.common().member_flags().IS_OPTIONAL(false);
    cst_alarms.common().member_flags().IS_MUST_UNDERSTAND(false);
    cst_alarms.common().member_flags().IS_KEY(false);
    cst_alarms.common().member_flags().IS_DEFAULT(false); // Doesn't apply
    cst_alarms.common().member_type_id(*TypeObjectFactory::get_instance()->get_sequence_identifier("Alarm", 0, true));


    cst_alarms.detail().name("alarms");

    type_object->complete().struct_type().member_seq().emplace_back(cst_alarms);


    // Header
    type_object->complete().struct_type().header().detail().type_name("DDSAlarmEx");
    // TODO inheritance


    TypeIdentifier identifier;
    identifier._d(EK_COMPLETE);

    SerializedPayload_t payload(static_cast<uint32_t>(
        CompleteStructType::getCdrSerializedSize(type_object->complete().struct_type()) + 4));
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload.data, payload.max_size);
    // Fixed endian (Page 221, EquivalenceHash definition of Extensible and Dynamic Topic Types for DDS document)
    eprosima::fastcdr::Cdr ser(
        fastbuffer, eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS,
        eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload.encapsulation = CDR_LE;

    type_object->serialize(ser);
    payload.length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    MD5 objectHash;
    objectHash.update((char*)payload.data, payload.length);
    objectHash.finalize();
    for(int i = 0; i < 14; ++i)
    {
        identifier.equivalence_hash()[i] = objectHash.digest[i];
    }

    TypeObjectFactory::get_instance()->add_type_object("DDSAlarmEx", &identifier, type_object);
    delete type_object;
    return TypeObjectFactory::get_instance()->get_type_object("DDSAlarmEx", true);
}
