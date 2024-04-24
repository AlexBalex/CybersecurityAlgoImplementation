#ifndef DES_H
#define DES_H

#include <stddef.h>

// Function to encrypt a block of data using DES
void des_encrypt(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key);

// Function to decrypt a block of data using DES
void des_decrypt(const unsigned char *ciphertext, unsigned char *plaintext, const unsigned char *key);

// DES round function
void des_round(unsigned char *block, const unsigned char *key);

#endif // DES_H