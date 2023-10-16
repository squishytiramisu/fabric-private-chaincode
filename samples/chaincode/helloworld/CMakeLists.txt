cmake_minimum_required(VERSION 3.5.1)

set(SOURCE_FILES
    helloworld_cc.cpp
    dto_json.cpp
    validate.cpp
    )



include($ENV{FPC_PATH}/ecc_enclave/enclave/CMakeLists-common-app-enclave.txt)

set(PYTHON_EXECUTABLE_PATH "${CMAKE_SOURCE_DIR}/python/elgamal")


# Add a custom target to run your Python executable
add_custom_target(run_python
    COMMAND ${PYTHON_EXECUTABLE_PATH}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Add a dependency on the custom target
add_dependencies(enclave run_python)
