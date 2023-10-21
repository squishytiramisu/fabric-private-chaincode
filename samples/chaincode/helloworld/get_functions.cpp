
#include "get_functions.h"


person_t getPerson(std::string key,  shim_ctx_ptr_t ctx)
{
    // check if person exists
    uint32_t person_bytes_len = 0;
    uint8_t person_bytes[MAX_VALUE_SIZE];
    get_state(key.c_str(), person_bytes, sizeof(person_bytes), &person_bytes_len, ctx);

    if (person_bytes_len == 0)
    {
        LOG_DEBUG("personCC: person does not exist");
        person_t the_person;
        return the_person;
    }

    // get person person from json
    person_t the_person;
    unmarshal_person(&the_person, (const char*)person_bytes, person_bytes_len);
    return the_person;
}

health_examination_t getHealthExamination(std::string id, shim_ctx_ptr_t ctx)
{

    uint32_t health_examination_len = 0;
    uint8_t health_examination[MAX_VALUE_SIZE];
    get_state("xdd", health_examination, sizeof(health_examination_len), &health_examination_len, ctx);

    if (health_examination_len == 0)
    {
        LOG_DEBUG("personCC: health examination does not exist");
        health_examination_t the_health_examination;
        return the_health_examination;
    }

    // get health examination from json
    health_examination_t the_health_examination;
    unmarshal_health_examination(&the_health_examination, (const char*)health_examination, health_examination_len);
    return the_health_examination;
}

life_insurance_t getLifeInsurance(std::string id, shim_ctx_ptr_t ctx)
{
    uint32_t life_insurance_len = 0;
    uint8_t life_insurance[MAX_VALUE_SIZE];
    get_state(id.c_str(), life_insurance, sizeof(life_insurance_len), &life_insurance_len, ctx);
    
    if (life_insurance_len == 0)
    {
        LOG_DEBUG("personCC: life insurance does not exist");
        life_insurance_t the_life_insurance;
        return the_life_insurance;
    }

    // get life insurance from json
    life_insurance_t the_life_insurance;
    unmarshal_life_insurance(&the_life_insurance, (const char*)life_insurance, life_insurance_len);
    return the_life_insurance;
}

work_permit_t getWorkPermit(std::string id, shim_ctx_ptr_t ctx)
{
    uint32_t work_permit_len = 0;
    uint8_t work_permit[MAX_VALUE_SIZE];
    get_state(id.c_str(), work_permit, sizeof(work_permit_len), &work_permit_len, ctx);

    if (work_permit_len == 0)
    {
        LOG_DEBUG("personCC: work permit does not exist");
        work_permit_t the_work_permit;
        return the_work_permit;
    }

    // get work permit from json
    work_permit_t the_work_permit;
    unmarshal_work_permit(&the_work_permit, (const char*)work_permit, work_permit_len);
    return the_work_permit;
}
