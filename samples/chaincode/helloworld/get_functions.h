#pragma once

#include <iostream>
#include <string>

#include "constants.h"
#include "shim.h"

#include "dto_json.h"


person_t getPerson(std::string key,  shim_ctx_ptr_t ctx);

health_examination_t getHealthExamination(std::string id,  shim_ctx_ptr_t ctx);

work_permit_t getWorkPermit(std::string id, shim_ctx_ptr_t ctx);

life_insurance_t getLifeInsurance(std::string id, shim_ctx_ptr_t ctx);