#ifndef RSA_H
#define RSA_H

#include <gmp.h>  // Required for GMP functions

// Define the structure for RSA keys
typedef struct {
    mpz_t n;  // Modulus for both the public and private keys
    mpz_t e;  // Public exponent
    mpz_t d;  // Private exponent
} RSAKey;

// Function prototypes for RSA key generation, encryption, and decryption
void generate_prime(mpz_t prime, int bit_size);
void generate_rsa_keys(RSAKey *key, int bit_size);
void rsa_encrypt(mpz_t c, const mpz_t m, const RSAKey *key);
void rsa_decrypt(mpz_t m, const mpz_t c, const RSAKey *key);

#endif // RSA_H