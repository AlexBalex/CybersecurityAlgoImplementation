#ifndef DES_H
#define DES_H

#include <stddef.h>
#include <stdint.h>

// Structure to hold the key schedule
struct des_ctx {
    uint64_t key_schedule[16]; // Storage for key schedule
};

// Function prototypes for DES operations
void des_key_setup(const uint8_t key[8], struct des_ctx *ctx);
void des_encrypt(struct des_ctx *ctx, const uint8_t plaintext[8], uint8_t ciphertext[8]);
void des_decrypt(struct des_ctx *ctx, const uint8_t ciphertext[8], uint8_t plaintext[8]);

#endif // DES_H