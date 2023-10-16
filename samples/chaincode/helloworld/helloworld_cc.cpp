#include "shim.h"
#include "logging.h"
#include <string>
#include "dto_json.h"
#include "validate.h"


#include <cstdio>
#include <iostream>
#include <string>

#define OK "OK"
#define NOT_FOUND "Asset not found"


#define MAX_VALUE_SIZE 1024

#define EXAMPLE_MAX_PARTICIPANTS 3
#define EXAMPLE_MIN_PARTICIPANTS 2

//  Add asset_name, value to ledger
std::string storeAsset(std::string asset_name, int value, shim_ctx_ptr_t ctx)
{
    LOG_DEBUG("HelloworldCC: +++ storeAsset +++");

    put_state(asset_name.c_str(), (uint8_t*)&value, sizeof(int), ctx);

    return OK;
}



std::string retrieveAsset(std::string asset_name, shim_ctx_ptr_t ctx)
{
    std::string result;
    LOG_DEBUG("HelloworldCC: +++ retrieveAsset +++");

    uint32_t asset_bytes_len = 0;
    uint8_t asset_bytes[MAX_VALUE_SIZE];
    get_state(asset_name.c_str(), asset_bytes, sizeof(asset_bytes), &asset_bytes_len, ctx);

    //  check if asset_name exists
    if (asset_bytes_len > 0)
    {
        result = asset_name +   ":" +  std::to_string((int)(*asset_bytes));
     } else {
        //  asset does not exist
        result = NOT_FOUND;
    }
    return result;
}

std::string own_obj_create(std::string own_obj_name,std::string caller, int value, shim_ctx_ptr_t ctx)
{
    uint32_t own_obj_bytes_len = 0;
    uint8_t own_obj_bytes[MAX_VALUE_SIZE];
    get_state(own_obj_name.c_str(), own_obj_bytes, sizeof(own_obj_bytes), &own_obj_bytes_len, ctx);

    if (own_obj_bytes_len > 0)
    {
        // own_obj already exists
        LOG_DEBUG("own_objCC: own_obj already exists");
        return "own_obj_ALREADY_EXISTING";
    }

    if(!assertValidTaj(caller)){
        return "INVALID TAJ";
    }

    // create new own_obj
    my_struct_t new_own_obj;
    new_own_obj.name = (char*)caller.c_str();
    new_own_obj.value = value;

    // convert to json string and store
    std::string json = marshal_my_struct(&new_own_obj);
    put_state(own_obj_name.c_str(), (uint8_t*)json.c_str(), json.size(), ctx);

    return OK;
}

my_struct_t get_struct_state(
    std::string key,  shim_ctx_ptr_t ctx)
{
    // check if struct already exists
    uint32_t struct_bytes_len = 0;
    uint8_t struct_bytes[MAX_VALUE_SIZE];
    get_state(key.c_str(), struct_bytes, sizeof(struct_bytes), &struct_bytes_len, ctx);

    if (struct_bytes_len == 0)
    {
        LOG_DEBUG("structCC: struct does not exist");
        my_struct_t the_struct;
        return the_struct;
    }

    // get struct struct from json
    my_struct_t the_struct;
    unmarshal_my_struct(&the_struct, (const char*)struct_bytes, struct_bytes_len);


    return the_struct;
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
    std::string asset_name = params[0];
    std::string result;


    char real_bidder_name_msp_id[1024];
    char real_bidder_name_dn[1024];
    get_creator_name(real_bidder_name_msp_id, sizeof(real_bidder_name_msp_id),
        real_bidder_name_dn, sizeof(real_bidder_name_dn), ctx);
    LOG_INFO("AuctionCC: real bidder '(msp_id: %s, dn: %s)'",
        real_bidder_name_msp_id, real_bidder_name_dn);


    if (function_name == "storeAsset")
    {
        int value = std::stoi (params[1]);
        result = storeAsset(asset_name, value, ctx);
    }
    else if (function_name == "retrieveAsset")
    {
        result = retrieveAsset(asset_name, ctx);
    }
    else if (function_name == "storeMyStruct")
    {
        int value = std::stoi (params[1]);
        result = own_obj_create(asset_name, params[0],value, ctx);
    }
    else if (function_name == "getMyStruct")
    {
        my_struct_t res = get_struct_state(asset_name, ctx);
        LOG_INFO("OBJ OWNER: %s", res.name);
        LOG_INFO("OBJ VALUE: %d", res.value);

        result = res.name;          

    }
    else
    {
        // unknown function
        LOG_DEBUG("HelloworldCC: RECEIVED UNKNOWN transaction '%s'", function_name);
        return -1;
    }

    // check that result fits into response
    int neededSize = result.size();
    if (max_response_len < neededSize)
    {
        // error:  buffer too small for the response to be sent
        LOG_DEBUG("HelloworldCC: Response buffer too small");
        *actual_response_len = 0;
        return -1;
    }

    // copy result to response
    memcpy(response, result.c_str(), neededSize);
    *actual_response_len = neededSize;
    LOG_DEBUG("HelloworldCC: Response: %s", result.c_str());
    LOG_DEBUG("HelloworldCC: +++ Executing done +++");
    return 0;
}






std::string invokePython(){
    const char* pythonCommand = "./python/elgamal";

    FILE* pipe = popen("echo Hello", "r");
    if (pipe == nullptr) {
        return "1";
    }

    char buffer[128];
    std::string result = "";

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    int status = pclose(pipe);

    return "SUCCESS";
}