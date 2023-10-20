/*
 * Copyright IBM Corp. All Rights Reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "dto_json.h"
#include "parson.h"

///////////// REAL DTOs

std::string marshal_person(person_t* person)
{
    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "id", person->id.c_str());
    json_object_set_string(root_object, "taj", person->taj.c_str());
    json_object_set_string(root_object, "name", person->name.c_str());
    json_object_set_string(root_object, "birth_date", person->birth_date.c_str());
    json_object_set_string(root_object, "death_date", person->death_date.c_str());
    char* serialized_string = json_serialize_to_string(root_value);
    std::string out(serialized_string);
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
    return out;
}

std::string marshal_health_examination(health_examination_t* health_examination){
    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "id", health_examination->id.c_str());
    json_object_set_string(root_object, "taj", health_examination->taj.c_str());
    json_object_set_number(root_object, "systole", health_examination->systole);
    json_object_set_number(root_object, "diastole", health_examination->diastole);
    json_object_set_string(root_object, "date", health_examination->date.c_str());
    char* serialized_string = json_serialize_to_string(root_value);
    std::string out(serialized_string);
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
    return out;
}

std::string marshal_life_insurance(life_insurance_t* life_insurance){
    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "id", life_insurance->id.c_str());
    json_object_set_string(root_object, "taj", life_insurance->taj.c_str());
    json_object_set_string(root_object, "from", life_insurance->from.c_str());
    json_object_set_string(root_object, "to", life_insurance->to.c_str());
    json_object_set_number(root_object, "cost", life_insurance->cost);
    json_object_set_number(root_object, "payment", life_insurance->payment);
    json_object_set_boolean(root_object, "should_pay", life_insurance->should_pay);
    char* serialized_string = json_serialize_to_string(root_value);
    std::string out(serialized_string);
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
    return out;
}

std::string marshal_work_permit(work_permit_t* work_permit){
    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "id", work_permit->id.c_str());
    json_object_set_string(root_object, "name", work_permit->name.c_str());
    json_object_set_string(root_object, "from", work_permit->from.c_str());
    json_object_set_string(root_object, "issuer", work_permit->issuer.c_str());
    char* serialized_string = json_serialize_to_string(root_value);
    std::string out(serialized_string);
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
    return out;
}

int unmarshal_person(person_t* person, const char* json_bytes, uint32_t json_len)
{
    JSON_Value* root = json_parse_string(json_bytes);
    person->id = json_object_get_string(json_object(root), "id");
    person->taj = json_object_get_string(json_object(root), "taj");
    person->name = json_object_get_string(json_object(root), "name");
    person->birth_date = json_object_get_string(json_object(root), "birth_date");
    person->death_date = json_object_get_string(json_object(root), "death_date");
    json_value_free(root);
    return 1;
}

int unmarshal_health_examination(health_examination_t* health_examination, const char* json_bytes, uint32_t json_len)
{
    JSON_Value* root = json_parse_string(json_bytes);
    health_examination->id = json_object_get_string(json_object(root), "id");
    health_examination->taj = json_object_get_string(json_object(root), "taj");
    health_examination->systole = json_object_get_number(json_object(root), "systole");
    health_examination->diastole = json_object_get_number(json_object(root), "diastole");
    health_examination->date = json_object_get_string(json_object(root), "date");
    json_value_free(root);
    return 1;
}

int unmarshal_life_insurance(life_insurance_t* life_insurance, const char* json_bytes, uint32_t json_len)
{
    JSON_Value* root = json_parse_string(json_bytes);
    life_insurance->id = json_object_get_string(json_object(root), "id");
    life_insurance->taj = json_object_get_string(json_object(root), "taj");
    life_insurance->from = json_object_get_string(json_object(root), "from");
    life_insurance->to = json_object_get_string(json_object(root), "to");
    life_insurance->cost = json_object_get_number(json_object(root), "cost");
    life_insurance->payment = json_object_get_number(json_object(root), "payment");
    life_insurance->should_pay = json_object_get_boolean(json_object(root), "should_pay");
    json_value_free(root);
    return 1;
}

int unmarshal_work_permit(work_permit_t* work_permit, const char* json_bytes, uint32_t json_len)
{
    JSON_Value* root = json_parse_string(json_bytes);
    work_permit->id = json_object_get_string(json_object(root), "id");
    work_permit->name = json_object_get_string(json_object(root), "name");
    work_permit->from = json_object_get_string(json_object(root), "from");
    work_permit->issuer = json_object_get_string(json_object(root), "issuer");
    json_value_free(root);
    return 1;
}