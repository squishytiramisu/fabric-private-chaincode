#include "validate.h"
#include "dto_json.h"
#include "shim.h"

#include <numeric>
#include <vector>
#include "constants.h"

template <typename T>
bool isNotNull(const T* ptr) {
    return ptr != nullptr;
}

bool isEmpty(const std::string& str) {
    return str.empty();
}

bool containsOnlyNumbers(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool hasLength(const std::string& str, size_t length) {
    return str.length() == length;
}

bool isValidTaj(const std::string& str){
    if(!isEmpty(str) && hasLength(str,9) && containsOnlyNumbers(str) ){
        return true;
    }
    return false;
}

bool isValidBirthDate(const std::string& str){
    if(!isEmpty(str) && hasLength(str,10) && containsOnlyNumbers(str) ){
        return true;
    }
    return false;
}


bool isValidName(const std::string& str){
    if(!isEmpty(str) && isShorterThan(str,100) && containsOnlyLetters(str) ){
        return true;
    }
    return false;
}

bool containsOnlyLetters(const std::string& str) {
    for (char c : str) {
        if (!std::isalpha(c)) {
            return false;
        }
    }
    return true;
}

bool isShorterThan(const std::string& str, size_t length) {
    return str.length() < length;
}

bool hasNoSpecialCharacters(const std::string& str) {
    for (char c : str) {
        if (!std::isalnum(c)) {
            return false;
        }
    }
    return true;
}

bool isValidDate(const std::string& str){
    if(hasLength(str,8) && containsOnlyNumbers(str) ){
        return true;
    }
    return false;
}

bool isValidCost(int cost){
    if(cost > 0){
        return true;
    }
    return false;
}

bool isValidPayment(int payment){
    if(payment > 0){
        return true;
    }
    return false;
}


// CTX ACCESS

bool tajExists(const std::string& str, shim_ctx_ptr_t ctx){
    return false;
    const std::string key(".P.");
    std::map<std::string, std::string> values;
    get_state_by_partial_composite_key(key.c_str(), values, ctx);

    for (auto person : values)
    {
        person_t person_obj;
        unmarshal_person(&person_obj, (const char*)person.second.c_str(), person.second.length());
        if(person_obj.taj == str){
            return true;
        }
    }
    return false;
}

bool isValidId(const std::string& str){
    if(!isEmpty(str) && hasLength(str,9) && hasNoSpecialCharacters(str) ){
        return true;
    }
    return false;
}

bool idExists(const std::string& id, shim_ctx_ptr_t ctx){
    uint32_t person_bytes_len = 0;
    uint8_t person_bytes[MAX_VALUE_SIZE];

    //Prefix with letter P
    get_state( (".P."+id).c_str(), person_bytes, sizeof(person_bytes), &person_bytes_len, ctx);

    if (person_bytes_len > 0)
    {
        LOG_DEBUG("personCC: person already exists");
        return true;
    }
}

bool isAlive(const std::string& id, shim_ctx_ptr_t ctx){
    uint32_t person_bytes_len = 0;
    uint8_t person_bytes[MAX_VALUE_SIZE];

    //Prefix with letter P
    get_state( (".P."+id).c_str(), person_bytes, sizeof(person_bytes), &person_bytes_len, ctx);

    if (person_bytes_len > 0)
    {
        person_t person_obj;
        unmarshal_person(&person_obj, (const char*)person_bytes, person_bytes_len);
        if(person_obj.death_date == ""){
            return true;
        }
    }
    return false;
}


bool isHealthy(const std::string& id, shim_ctx_ptr_t ctx){
    uint32_t health_examination_len = 0;
    uint8_t health_examination[MAX_VALUE_SIZE];

    //Prefix with letter H
    get_state( (".H."+id).c_str(), health_examination, sizeof(health_examination_len), &health_examination_len, ctx);

    if (health_examination_len > 0)
    {
        health_examination_t health_examination_obj;
        unmarshal_health_examination(&health_examination_obj, (const char*)health_examination, health_examination_len);
        if(health_examination_obj.systole < 140 && health_examination_obj.diastole < 90){
            return true;
        }
    }
    return false;
}

bool hasLifeInsurance(const std::string& id, shim_ctx_ptr_t ctx){
    uint32_t life_insurance_len = 0;
    uint8_t life_insurance[MAX_VALUE_SIZE];

    //Prefix with letter L
    get_state( (".L."+id).c_str(), life_insurance, sizeof(life_insurance_len), &life_insurance_len, ctx);

    if (life_insurance_len > 0)
    {
        life_insurance_t life_insurance_obj;
        unmarshal_life_insurance(&life_insurance_obj, (const char*)life_insurance, life_insurance_len);
        //TODO DATE CHECK
        return true;
    }

    return false;
}

bool hasWorkPermit(const std::string& id, shim_ctx_ptr_t ctx){
    uint32_t work_permit_len = 0;
    uint8_t work_permit[MAX_VALUE_SIZE];

    //Prefix with letter W
    get_state( (".W."+id).c_str(), work_permit, sizeof(work_permit_len), &work_permit_len, ctx);

    if (work_permit_len > 0)
    {
        work_permit_t work_permit_obj;
        unmarshal_work_permit(&work_permit_obj, (const char*)work_permit, work_permit_len);
        //TODO DATE CHECK
        return true;
    }

    return false;
}