

#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

#include <iostream>
#include <string>

template <typename T>
bool isNotNull(const T* ptr);

bool isEmpty(const std::string& str);

bool containsOnlyNumbers(const std::string& str);

bool hasLength(const std::string& str, size_t length);

bool assertValidTaj(const std::string& str);

#endif // MYFUNCTIONS_H
