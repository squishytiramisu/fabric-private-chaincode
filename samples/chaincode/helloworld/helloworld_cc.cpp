#include "shim.h"
#include "logging.h"
#include <string>
#include "dto_json.h"
#include "validate.h"
#include "complex_validations.h"


#include "get_functions.h"
#include "constants.h"

#include "elgamal.h"
#include "events.h"



void prepareResult(std::string result, uint8_t* response, uint32_t max_response_len, uint32_t* actual_response_len){
    // check that result fits into response
    int neededSize = result.size();
    if (max_response_len < neededSize)
    {
        // error:  buffer too small for the response to be sent
        LOG_DEBUG("HelloworldCC: Response buffer too small");
        *actual_response_len = 0;
        result = "ERROR: Response buffer too small";
    }

    // copy result to response
    memcpy(response, result.c_str(), neededSize);
    *actual_response_len = neededSize;
}



// implements chaincode logic for invoke
int invoke(
    uint8_t* response,
    uint32_t max_response_len,
    uint32_t* actual_response_len,
    shim_ctx_ptr_t ctx)
{
    LOG_DEBUG("HelloworldCC: +++ Executing helloworld chaincode invocation +++");

    std::string function_name;
    std::vector<std::string> params;
    get_func_and_params(function_name, params, ctx);
    std::string result;


    char real_bidder_name_msp_id[1024];
    char real_bidder_name_dn[1024];
    get_creator_name(real_bidder_name_msp_id, sizeof(real_bidder_name_msp_id),
        real_bidder_name_dn, sizeof(real_bidder_name_dn), ctx);
    LOG_INFO("AuctionCC: real bidder '(msp_id: %s, dn: %s)'",
        real_bidder_name_msp_id, real_bidder_name_dn);


    if(!isAllowedToInvoke( signature, real_bidder_name_msp_id, real_bidder_name_dn, ctx)){
        LOG_DEBUG("HelloworldCC: Not allowed to invoke");
        result = "ERROR: Not allowed to invoke";
        prepareResult(result, response, max_response_len, actual_response_len);
        return 0;
    }

    /////////////// FUNCTION SWITCH ///////////////////////

    if(function_name == "dummyEncrypt"){
        if(params.size() != 1){
            LOG_DEBUG("personCC: dummyEncrypt: Wrong number of arguments");
            result = "ERROR: Wrong number of arguments";
            prepareResult(result, response, max_response_len, actual_response_len);
            return 0;
        }

        std::string input = params[0];
        result = dummyEncrypt(input);
        prepareResult(result, response, max_response_len, actual_response_len);
        return 0;
    }

    if(function_name == "PersonBorn"){
        if(params.size() != 4){
            LOG_DEBUG("personCC: PersonBorn: Wrong number of arguments");
            result = "ERROR: Wrong number of arguments";
            prepareResult(result, response, max_response_len, actual_response_len);
            return 0;
        }
        std::string id = params[0];
        std::string taj = params[1];
        std::string name = params[2];
        std::string birth_date = params[3];
        result = personBorn(ctx, id, taj, name, birth_date);

        std::string aha = "aha";

        put_public_state("aha", (uint8_t*)aha.c_str(), aha.length(), ctx);
        put_public_state("sdf", (uint8_t*)aha.c_str(), aha.length(), ctx);
    

        prepareResult(result, response, max_response_len, actual_response_len);
        return 0;

    }else if(function_name == "PersonDie"){
        if(params.size() != 1){
            LOG_DEBUG("personCC: PersonDie: Wrong number of arguments");
            result = "ERROR: Wrong number of arguments";
            prepareResult(result, response, max_response_len, actual_response_len);
            return 0;
        }
        std::string id = params[0];
        result = personDie(ctx, id);
        prepareResult(result, response, max_response_len, actual_response_len);
        return 0;
    }else if(function_name == "IssueHealthExamination"){
        if(params.size() != 5){
            result = "ERROR: Wrong number of arguments";
            prepareResult(result, response, max_response_len, actual_response_len);
            LOG_DEBUG("personCC: IssueHealthExamination: Wrong number of arguments");
            return 0;
        }
        std::string id = params[0];
        std::string taj = params[1];
        std::string examination_date = params[2];
        int systole = std::stoi(params[3]);
        int diastole = std::stoi(params[4]);
        result = issueHealthExamination(ctx, id, taj, examination_date, systole, diastole);
        prepareResult(result, response, max_response_len, actual_response_len);
        return 0;

    }else if(function_name == "IssueLifeInsurance"){
        if(params.size() != 6){
            LOG_DEBUG("personCC: IssueLifeInsurance: Wrong number of arguments");
            result = "ERROR: Wrong number of arguments";
            prepareResult(result, response, max_response_len, actual_response_len);
            return 0;
        }
        std::string id = params[0];
        std::string taj = params[1];
        std::string from = params[2];
        std::string to = params[3];
        int cost = std::stoi(params[4]);
        int payment = std::stoi(params[5]);
        bool should_pay = false;

        result = issueLifeInsurance(ctx, id, taj, from, to, cost, payment, false);
        prepareResult(result, response, max_response_len, actual_response_len);
        return 0;
    }else if(function_name == "IssueWorkPermit"){
        if(params.size() != 4){
            LOG_DEBUG("personCC: IssueWorkPermit: Wrong number of arguments");
            result = "ERROR: Wrong number of arguments";
            prepareResult(result, response, max_response_len, actual_response_len);
            return 0;
        }
        std::string id = params[0];
        std::string name = params[1];
        std::string from = params[2];
        std::string issuer = params[3];
        result = issueWorkPermit(ctx, id, name, from, issuer);
        prepareResult(result, response, max_response_len, actual_response_len);
        return 0;
    }else if(function_name == "RevokeWorkPermit"){
        if(params.size() != 1){
            LOG_DEBUG("personCC: RevokeWorkPermit: Wrong number of arguments");
            result = "ERROR: Wrong number of arguments";
            prepareResult(result, response, max_response_len, actual_response_len);
            return 0;
        }
        std::string id = params[0];
        result = revokeWorkPermit(ctx, id);
    }else if(function_name == "hasWorkPermit")
    {
        if(params.size() != 1){
            LOG_DEBUG("personCC: hasWorkPermit: Wrong number of arguments");
            result = "ERROR: Wrong number of arguments";
            prepareResult(result, response, max_response_len, actual_response_len);
            return 0;
        }
        std::string id = params[0];
        result = canWork(ctx, id);
        prepareResult(result, response, max_response_len, actual_response_len);
        return 0;
    }
    else if( function_name == "getPerson"){
        if(params.size() != 1){
            LOG_DEBUG("personCC: getPerson: Wrong number of arguments");
            result = "ERROR: Wrong number of arguments";
            prepareResult(result, response, max_response_len, actual_response_len);
            return 0;
        }

        std::string id = params[0];
        person_t the_person = getPerson((id).c_str(), ctx);
        result = marshal_person(&the_person);
        prepareResult(result, response, max_response_len, actual_response_len);
        return 0;
    }
    else if( function_name == "getHealthExamination"){
        if(params.size() != 1){
            LOG_DEBUG("personCC: getHealthExamination: Wrong number of arguments");
            result = "ERROR: Wrong number of arguments";
            prepareResult(result, response, max_response_len, actual_response_len);
            return 0;
        }

        std::string id = params[0];
        health_examination_t the_health_examination = getHealthExamination((id).c_str(), ctx);
        result = marshal_health_examination(&the_health_examination);
        prepareResult(result, response, max_response_len, actual_response_len);
        return 0;
    }
    else if( function_name == "getLifeInsurance"){
        if(params.size() != 1){
            LOG_DEBUG("personCC: getLifeInsurance: Wrong number of arguments");
            result = "ERROR: Wrong number of arguments";
            prepareResult(result, response, max_response_len, actual_response_len);
            return 0;

        }
        std::string id = params[0];
        life_insurance_t the_life_insurance = getLifeInsurance(id.c_str(), ctx);
        result = marshal_life_insurance(&the_life_insurance);
        prepareResult(result, response, max_response_len, actual_response_len);
        return 0;
    }
    else if( function_name == "getWorkPermit"){
        if(params.size() != 1){
            LOG_DEBUG("personCC: getWorkPermit: Wrong number of arguments");
            result = "ERROR: Wrong number of arguments";
            prepareResult(result, response, max_response_len, actual_response_len);
            return 0;

        }
        std::string id = params[0];
        work_permit_t the_work_permit = getWorkPermit((id).c_str(), ctx);


        result = marshal_work_permit(&the_work_permit);
        uint32_t datamap_bytes_len = 0;
        uint8_t datamap_bytes[MAX_VALUE_SIZE];

        get_public_state("aha", datamap_bytes, sizeof(datamap_bytes), &datamap_bytes_len, ctx);
        std::string aha = (const char*)datamap_bytes;


        prepareResult(aha, response, max_response_len, actual_response_len);
        return 0;
    }
    else{
        LOG_DEBUG("HelloworldCC: RECEIVED UNKNOWN transaction '%s'", function_name);
        return 0;
    }
    /////////////// RETURNING ///////////////////////

    prepareResult(result, response, max_response_len, actual_response_len);

    LOG_DEBUG("HelloworldCC: Response: %s", result.c_str());
    LOG_DEBUG("HelloworldCC: +++ Executing done +++");
    return 0;
}


