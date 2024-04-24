#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <stddef.h>

// Constants for AES-128
#define AES_KEY_SIZE 16  // AES-128 uses a 128-bit key
#define AES_BLOCK_SIZE 16  // AES block size is 128 bits
#define AES_ROUNDS 10  // Number of rounds in AES-128

// Function prototypes for AES encryption and decryption
void AES_Encrypt(const uint8_t* plaintext, const uint8_t* key, uint8_t* ciphertext, int keySize, int rounds);
void AES_Decrypt(const uint8_t* ciphertext, const uint8_t* key, uint8_t* plaintext, int keySize, int rounds);

#endif // AES_H