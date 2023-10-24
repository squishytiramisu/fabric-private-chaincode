#pragma once


#include <openssl/bn.h>
#include <string>
#include <tuple>


std::string elgamal_encrypt(BIGNUM* g, BIGNUM* g_hat, BIGNUM* q, BIGNUM* p, BIGNUM* y, const std::string& m);

std::string intToHex(int value);

int hexToInt(const std::string& hex);

std::string xorHashAndString(const unsigned char* hash, const std::string& str);