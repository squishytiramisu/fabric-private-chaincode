//#include <iostream>
#include <string>
#include <sstream>
#include <openssl/sha.h>
#include <openssl/bn.h>
#include <cstring>

#include "elgamal.h"

#include <vector>

// Encryption function
std::string elgamal_encrypt(BIGNUM* g, BIGNUM* g_hat, BIGNUM* q, BIGNUM* p, BIGNUM* y, const std::string& m) {
std::string aha = "0";
try
{
    /* code */
    BN_CTX* ctx = BN_CTX_new();
    
    BIGNUM* r = BN_new();
    BIGNUM* s = BN_new();
    BIGNUM* rG = BN_new();
    BIGNUM* h = BN_new();
    BIGNUM* u = BN_new();
    BIGNUM* w = BN_new();
    BIGNUM* u_hat = BN_new();
    BIGNUM* w_hat = BN_new();
    BIGNUM* e = BN_new();
    BIGNUM* f = BN_new();

    // Generate random values for r and s
    BN_dec2bn(&r, "888729205483307");
    //BN_rand_range(r, q);

    BN_dec2bn(&s, "1016923619211339");
    //BN_rand_range(s, q);



    // Calculate rG = y^r mod p
    BN_mod_exp(rG, y, r, p, ctx);

    

    ///////////////////////GOOD UNTIL HERE//////////////////////////
 
    // Convert rG to a hex string
    char* rG_hex = BN_bn2hex(rG);
    std::string rG_dec = BN_bn2dec(rG);

    std::vector<unsigned char> utf8_bytes(rG_dec.begin(), rG_dec.end());



    // Hash the hex string using SHA-256
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)rG_hex, strlen(rG_hex), hash);


    // Convert the hash to a BIGNUM
    BN_bin2bn(hash, SHA256_DIGEST_LENGTH, h);


    // THIS IS OKAY
    // Convert the plaintext message to a hex string
    std::string m_hex;
    for (char c : m) {
        m_hex += intToHex((int)c);
    }

    // XOR the hashed value with the message
    /*std::string c_hex;
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        c_hex += intToHex((int)hash[i] ^ hexToInt(m_hex.substr(i * 2, 2)));
    }
    */
    //std::string c_hex = xorHashAndString(hash, m_hex);  

    m_hex =          "68656c6c6f";
    std::string ha = "9774404e60351a33a9fbfd7706ebc2f1724cac54fe64166d708205c0d9e7c7df";

    

    BIGNUM *result_bn = BN_new();

    // Convert hexadecimal strings to BIGNUM
    BIGNUM *m_bn = BN_new();
    BIGNUM *h_bn = BN_new();
    BN_hex2bn(&m_bn, m_hex.c_str());
    BN_hex2bn(&h_bn, ha.c_str());

    // Get the binary representations of the BIGNUMs
    unsigned char *m_bin = static_cast<unsigned char*>(OPENSSL_malloc(BN_num_bytes(m_bn)));
    unsigned char *h_bin = static_cast<unsigned char*>(OPENSSL_malloc(BN_num_bytes(h_bn)));
    BN_bn2bin(m_bn, m_bin);
    BN_bn2bin(h_bn, h_bin);

    // Perform the XOR operation
    unsigned char *result_bin = static_cast<unsigned char*>(OPENSSL_malloc(std::max(BN_num_bytes(m_bn), BN_num_bytes(h_bn))));
    for (int i = 0; i < std::max(BN_num_bytes(m_bn), BN_num_bytes(h_bn)); i++) {
        result_bin[i] = static_cast<unsigned char>(m_bin[i] ^ h_bin[i]);
    }

    // Convert the result back to a BIGNUM
    BN_bin2bn(result_bin, std::max(BN_num_bytes(m_bn), BN_num_bytes(h_bn)), result_bn);

    // Convert the result back to a hexadecimal string
    char *resultHex = BN_bn2hex(result_bn);

    std::string c_hex(resultHex);




    OPENSSL_free(resultHex);
    BN_free(result_bn);

    return c_hex;

    // Calculate u = g^r mod p
    BN_mod_exp(u, g, r, p, ctx);
    aha = "2";
    // Calculate w = g^s mod p
    BN_mod_exp(w, g, s, p, ctx);

    aha = "3";
    // Calculate u_hat = g_hat^r mod p
    BN_mod_exp(u_hat, g_hat, r, p, ctx);

    aha = "4";
    // Calculate w_hat = g_hat^s mod p
    BN_mod_exp(w_hat, g_hat, s, p, ctx);



    aha = "5";
    // Calculate e = hash_2(c, u, w, w_hat, u_hat, q)
    BN_mod_mul(e, u, u_hat, p, ctx);
    BN_mod_mul(e, e, w, p, ctx);
    BN_mod_mul(e, e, w_hat, p, ctx);
    BN_mod_mul(e, e, u_hat, p, ctx);
    BN_mod_mul(e, e, h, p, ctx);

    aha = "6";
    BN_mod(e, e, q, ctx);

    // Calculate f = (s + r * e) % q
    BN_mul(r, r, e, ctx);
    BN_mod_add(f, s, r, q, ctx);

    aha = "7";



    // Return the encrypted values as hex strings
    char* c = BN_bn2hex(e);

    


    aha = "8";
    char* u_str = BN_bn2dec(u);
    
    aha = "9";
    char* u_hat_str = BN_bn2dec(u_hat);
    char* e_str = BN_bn2dec(e);
    char* f_str = BN_bn2dec(f);

    //CHECKPOINT EDDIG JO

    aha = "10";
    //BN_CTX_end(ctx);
    BN_CTX_free(ctx);
    BN_free(r);
    BN_free(s);
    BN_free(rG);
    BN_free(h);
    BN_free(u);
    BN_free(w);
    BN_free(u_hat);
    BN_free(w_hat);
    BN_free(e);
    BN_free(f);
    aha = "11";


    return std::string(c) + " " + std::string(u_str) + " " + std::string(u_hat_str) + " " + std::string(e_str) + " " + std::string(f_str);
}
catch(const std::exception& e)
{
    return aha;
}
}

std::string intToHex(int value) {
    try {
        std::string hexString;
        while (value > 0) {
            int digit = value % 16;
            char hexDigit = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
            hexString = hexDigit + hexString;
            value /= 16;
        }
        if (hexString.empty()) {
            hexString = "0";
        }
        return hexString;
    } catch (const std::exception& e) {
        // Handle the exception (e.g., log the error, rethrow, or return a default value)
        // In this example, we simply rethrow the exception.
        throw;
    }
}

// Helper function to convert a hexadecimal string to an integer
int hexToInt(const std::string& hex) {
    try {
        int value = 0;
        for (char c : hex) {
            value *= 16;
            if (c >= '0' && c <= '9') {
                value += (c - '0');
            } else if (c >= 'A' && c <= 'F') {
                value += (c - 'A' + 10);
            } else if (c >= 'a' && c <= 'f') {
                value += (c - 'a' + 10);
            } else {
                // Handle invalid characters if necessary
                // In this example, it simply ignores invalid characters
            }
        }
        return value;
    } catch (const std::exception& e) {
        // Handle the exception (e.g., log the error, rethrow, or return a default value)
        // In this example, we simply rethrow the exception.
        throw;
    }
}

std::string xorHashAndString(const unsigned char* hash, const std::string& str) {
    try {
        size_t hashLength = SHA256_DIGEST_LENGTH;
        if (hashLength * 2 != str.size()) {
            // Handle size mismatch if necessary
            // In this example, we return an empty string.
            return "";
        }

        std::string result;
        for (size_t i = 0; i < hashLength; i++) {
            // Convert each byte of hash to a hexadecimal string and XOR with the corresponding character in str
            result += intToHex(static_cast<int>(hash[i] ^ hexToInt(str.substr(i * 2, 2))));
        }
        return result;
    } catch (const std::exception& e) {
        // Handle the exception (e.g., log the error, rethrow, or return a default value)
        // In this example, we simply rethrow the exception.
        throw;
    }
}