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



    // SECTION COMPUTE R S RG GOOD
    // Generate random values for r and s
    BN_dec2bn(&r, "17373");
    //BN_rand_range(r, q);

    BN_dec2bn(&s, "32319");
    //BN_rand_range(s, q);

    // Calculate rG = y^r mod p
    BN_mod_exp(rG, y, r, p, ctx);

 
    // Convert rG to a hex string
    char* rG_hex = BN_bn2hex(rG);
    std::string rG_dec = BN_bn2dec(rG);

    // END SECTION COMPUTE R S RG

    const unsigned char* utf8_rG = reinterpret_cast<const unsigned char*>(rG_dec.c_str());
    

    // Hash the hex string using SHA-256
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)utf8_rG, rG_dec.length(), hash);
    BN_bin2bn(hash, SHA256_DIGEST_LENGTH, h);




    // COMPUTE M HEX GOOD
    std::string m_hex;
    for (char c : m) {
        m_hex += intToHex((int)c);
    }
    // END COMPUTE M HEX


    std::string ha = BN_bn2hex(h);

    


    // SECTION COMPUTE C
    BIGNUM *m_bn = BN_new();
    BIGNUM *h_bn = BN_new();
    BN_hex2bn(&m_bn, m_hex.c_str());
    BN_hex2bn(&h_bn, ha.c_str());

    size_t max_len = std::max(BN_num_bytes(m_bn), BN_num_bytes(h_bn));
    unsigned char *m_bin = static_cast<unsigned char *>(OPENSSL_malloc(max_len));
    unsigned char *h_bin = static_cast<unsigned char *>(OPENSSL_malloc(max_len));
    BN_bn2binpad(m_bn, m_bin, max_len);
    BN_bn2binpad(h_bn, h_bin, max_len);
    // Perform the XOR operation byte-wise
    unsigned char *result_bin = static_cast<unsigned char *>(OPENSSL_malloc(max_len));
    for (size_t i = 0; i < max_len; i++) {
        result_bin[i] = m_bin[i] ^ h_bin[i];
    }
    // Convert the result back to a BIGNUM
    BIGNUM *result_bn = BN_bin2bn(result_bin, max_len, nullptr);
    // Convert the result back to a hexadecimal string
    char *resultHex = BN_bn2hex(result_bn);

    std::string c_hex(resultHex);
    
    for(auto& c : c_hex)
    {
        c = tolower(c);
    }

    OPENSSL_free(resultHex);
    BN_free(result_bn);
    // END SECTION COMPUTE C



    // Calculate u = g^r mod p
    BN_mod_exp(u, g, r, p, ctx);
    // Calculate w = g^s mod p
    BN_mod_exp(w, g, s, p, ctx);
    // Calculate u_hat = g_hat^r mod p
    BN_mod_exp(u_hat, g_hat, r, p, ctx);
    // Calculate w_hat = g_hat^s mod p
    BN_mod_exp(w_hat, g_hat, s, p, ctx);


    // return u, w, u_hat, w_hat
    //GOOD
    char* u_dec = BN_bn2dec(u);
    char* w_dec = BN_bn2dec(w);
    char* u_hat_dec = BN_bn2dec(u_hat);
    char* w_hat_dec = BN_bn2dec(w_hat);







    // Calculate e = hash_2(c, u, w, w_hat, u_hat, q)
    hash_2(c_hex, u, w, w_hat, u_hat, q,ctx,e);

    //return e
    //return BN_bn2dec(e);

    // Calculate f = (s + r * e) % q
    BN_mul(r, r, e, ctx);
    BN_mod_add(f, s, r, q, ctx);





    char* u_str = BN_bn2dec(u);
    char* u_hat_str = BN_bn2dec(u_hat);
    char* e_str = BN_bn2dec(e);
    char* f_str = BN_bn2dec(f);

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
    return std::string(c_hex) + " " + std::string(u_str) + " " + std::string(u_hat_str) + " " + std::string(e_str) + " " + std::string(f_str);
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
            char hexDigit = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
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





void hash_2(const std::string& hx, const BIGNUM* g1, const BIGNUM* g2, const BIGNUM* g3, const BIGNUM* g4, const BIGNUM* q,BN_CTX* xd, BIGNUM* result) {
    // Convert BIGNUMs to hexadecimal strings
    char* g1_dec = BN_bn2dec(g1);
    char* g2_dec = BN_bn2dec(g2);
    char* g3_dec = BN_bn2dec(g3);
    char* g4_dec = BN_bn2dec(g4);

    // Concatenate the input values as a single string
    std::string input_str = hx + g1_dec + g2_dec + g3_dec + g4_dec;

    // Convert the input string to UTF-8
    /*std::string input_str_utf8;
    for (char c : input_str) {
        input_str_utf8 += intToHex((int)c);
    }*/

    // Compute the SHA-256 hash
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(input_str.c_str()), input_str.length(), hash);

    // Convert the hash to a BIGNUM
    BN_bin2bn(hash, SHA256_DIGEST_LENGTH, result);

    // Reduce the hash modulo q
    BN_mod(result, result, q, xd);
}