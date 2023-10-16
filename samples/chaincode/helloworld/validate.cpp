#include "validate.h"

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

bool assertValidTaj(const std::string& str){
    if(!isEmpty(str) && hasLength(str,9) && containsOnlyNumbers(str) ){
        return true;
    }
    return false;
}