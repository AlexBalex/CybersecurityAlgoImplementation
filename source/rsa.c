#include <gmp.h> 
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../include/rsa.h"
#define EXIT_FAILURE 1

void generate_prime(mpz_t prime, int bit_size) {

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));  // Seed with current time
    mpz_urandomb(prime, state, bit_size);  // Generate a random number of 'bit_size' bits
    mpz_nextprime(prime, prime);  // Find the next prime greater than the random number
    gmp_randclear(state);
}

void generate_rsa_keys(RSAKey *key, int bit_size) {
    mpz_t p, q, phi;
    mpz_inits(p, q, phi, key->n, key->e, key->d, NULL);
    generate_prime(p, bit_size);
    generate_prime(q, bit_size);
    mpz_mul(key->n, p, q);  // n = p * q
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi, p, q);  // phi = (p-1) * (q-1)
    mpz_set_ui(key->e, 65537);  // Commonly used prime exponent
    if (mpz_gcd_ui(NULL, key->e, mpz_get_ui(phi)) != 1) {
        printf("Error: 'e' is not coprime with 'phi'.\n");
        exit(EXIT_FAILURE);
    }

    if (!mpz_invert(key->d, key->e, phi)) {
        printf("Error: Inverse of 'e' modulo 'phi' could not be computed.\n");
        exit(EXIT_FAILURE);
    }
    mpz_clears(p, q, phi, NULL);
}

void rsa_encrypt(mpz_t c, const mpz_t m, const RSAKey *key) {
    // c = m^e mod n
    mpz_powm(c, m, key->e, key->n);
}

void rsa_decrypt(mpz_t m, const mpz_t c, const RSAKey *key) {
    // m = c^d mod n
    mpz_powm(m, c, key->d, key->n);
}