#pragma once


#include <string>
#include "shim.h"

bool isAllowedToInvoke( std::string function,
                        std::string caller,
                        std::string caller_msp,
                        std::string signature,
                        shim_ctx_ptr_t ctx
                      );

