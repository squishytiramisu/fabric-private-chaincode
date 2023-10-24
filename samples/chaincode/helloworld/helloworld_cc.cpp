#include "shim.h"
#include "logging.h"
#include <string>
#include "dto_json.h"
#include "validate.h"
#include "complex_validations.h"


#include "get_functions.h"
#include "constants.h"

//#include "crypto.h"

//#include "../../../common/crypto/pdo/common/crypto/crypto_shared.h"



#include "elgamal.h"


const std::string VALID_TAJ = "123456789";

std::string dummyEncrypt(std::string input){


//params: g: 74564094343119386, g_hat: 77929355568141139, q: 1073067082236043, p: 109452842388076387, y: 8169135570303051890, m: hello
//elgamal_encrypt(BIGNUM* g, BIGNUM* g_hat, BIGNUM* q, BIGNUM* p, BIGNUM* y, const std::string& m);
    
    std::string g = "160172861198496142";
    std::string g_hat = "12167346400598986";
    std::string q = "1028101291338439";
    std::string p = "257025322834609751";
    std::string y = "123457389917166958";
    std::string m = "hello";

    BIGNUM* g_bn = BN_new();
    BIGNUM* g_hat_bn = BN_new();
    BIGNUM* q_bn = BN_new();
    BIGNUM* p_bn = BN_new();
    BIGNUM* y_bn = BN_new();
    BIGNUM* m_bn = BN_new();

    BN_dec2bn(&g_bn, g.c_str());
    BN_dec2bn(&g_hat_bn, g_hat.c_str());
    BN_dec2bn(&q_bn, q.c_str());
    BN_dec2bn(&p_bn, p.c_str());
    BN_dec2bn(&y_bn, y.c_str());
    BN_dec2bn(&m_bn, m.c_str());

    
    std::string encrypted = elgamal_encrypt(g_bn, g_hat_bn, q_bn, p_bn, y_bn, m);
    return encrypted;

}


std::string personBorn(shim_ctx_ptr_t ctx, std::string id, std::string taj, std::string name, std::string birth_date){


    //Validate request
    if(!validPersonBorn(id, taj, name, birth_date, ctx)){
        return "ERROR: Invalid request";
    }

    // create new person
    person_t new_person;
    new_person.id = (char*)id.c_str();
    new_person.taj = (char*)taj.c_str();
    new_person.name = (char*)name.c_str();
    new_person.birth_date = (char*)birth_date.c_str();
    new_person.death_date = "";

    return putPerson(new_person, ctx);
}

std::string personDie(shim_ctx_ptr_t ctx, std::string id){

    // check if person already exists
    if(!validPersonDie(id, ctx )){
        return "ERROR: Person does not exist or already dead";
    }

    person_t the_person = getPerson((id).c_str(), ctx);
    the_person.death_date = "TODO";
    std::string json = marshal_person(&the_person);
    put_state((id).c_str(), (uint8_t*)json.c_str(), json.size(), ctx);
    
    return OK;
}

std::string issueHealthExamination(shim_ctx_ptr_t ctx, std::string id, std::string taj, std::string examination_date, int systole, int diastole){

    std::string validFuncName = "";
    if (!isValidTaj(taj)) {
        validFuncName = "isValidTaj";
    } else if (!isValidId(id)) {
        validFuncName = "isValidId";
    } else if (!isValidDate(examination_date)) {
        validFuncName = "isValidDate";
    } else if (!idExists(id, ctx)) {
        validFuncName = "idExists";
    } else if (!isAlive(id, ctx)) {
        validFuncName = "isAlive";
    }

    if (!validFuncName.empty()) {
        return validFuncName;
    }
        
    health_examination_t new_examination;
    new_examination.date = (char*)examination_date.c_str();
    new_examination.systole = systole;
    new_examination.diastole = diastole;
    new_examination.taj = (char*)taj.c_str();
    new_examination.id = (char*)id.c_str();
        

    return putHealthExamination(new_examination, ctx);
}


std::string issueLifeInsurance(shim_ctx_ptr_t ctx, std::string id, std::string taj, std::string from, std::string to, int cost, int payment, bool should_pay){

    // check if person already exists
    if(!idExists(id, ctx)){
        return "ERROR: Person does not exist";
    }
    if(!isAlive(id, ctx)){
        return "ERROR: Person is not alive";
    }

    //NOT WORKING
    if(!isHealthy(id, ctx)){
        return "ERROR: Person is not healthy";
    }
        
    life_insurance_t new_life_insurance;

    new_life_insurance.id = (char*)id.c_str();
    new_life_insurance.taj = (char*)taj.c_str();
    new_life_insurance.from = (char*)from.c_str();
    new_life_insurance.to = (char*)to.c_str();

    new_life_insurance.cost = cost;
    new_life_insurance.payment = payment;
    new_life_insurance.should_pay = should_pay;


    return putLifeInsurance(new_life_insurance, ctx);
}

std::string issueWorkPermit(shim_ctx_ptr_t ctx, std::string id, std::string name, std::string from, std::string issuer){

    // check if person already exists
    if(!validIssueWorkPermit(id, name, from, issuer, ctx)){
        return "ERROR: Person does not exist or is not eligible for work permit";
    }
        
    work_permit_t new_work_permit;
    new_work_permit.id = (char*)id.c_str();
    new_work_permit.name = (char*)name.c_str();
    new_work_permit.from = (char*)from.c_str();
    new_work_permit.issuer = (char*)issuer.c_str();
        
    return putWorkPermit(new_work_permit, ctx);
}


std::string revokeWorkPermit(shim_ctx_ptr_t ctx, std::string id){
    
        // check if person already exists
        if(!validRevokeWorkPermit(id, ctx)){
            return "ERROR: Person does not exist or is not eligible for work permit";
        }

        work_permit_t the_work_permit = getWorkPermit((id).c_str(), ctx);
        the_work_permit.from = "";
        std::string json = marshal_work_permit(&the_work_permit);
        put_state((id).c_str(), (uint8_t*)json.c_str(), json.size(), ctx);

        return OK;
}

std::string canWork(shim_ctx_ptr_t ctx, std::string id){
    if(!validCanWork(id, ctx)){
        return "ERROR: Person does not exist";
    }

    if(hasWorkPermit(id, ctx)){
        return "NO WORK PERMIT";
    }

    return "HAS WORK PERMIT";
}

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


