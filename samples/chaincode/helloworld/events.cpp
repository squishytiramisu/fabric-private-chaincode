#include "events.h"



std::string dummyEncrypt(std::string input){  
    std::string g = "23761";
    std::string g_hat = "18425";
    std::string q = "12329";
    std::string p = "24659";
    std::string y = "176";
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

    if(!validPersonDie(id, ctx )){
        return "ERROR: Person does not exist or already dead";
    }
    person_t the_person = getPerson((id).c_str(), ctx);
    the_person.death_date = "TODO";
    return putPerson(the_person, ctx);    
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
