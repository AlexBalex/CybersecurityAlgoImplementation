#include "../include/des.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

// Define the expansion table
static const int EXPANSION_TABLE[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1};

static const int S_BOX[8][4][16] = {
    {// S1
     {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    {// S2
     {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    {// S3
     {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    {// S4
     {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
     {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    {// S5
     {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    {// S6
     {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    {// S7
     {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    {// S8
     {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

// Define the permutation table (P-box)
static const int PERMUTATION_TABLE[32] = {
    16,
    7,
    20,
    21,
    29,
    12,
    28,
    17,
    1,
    15,
    23,
    26,
    5,
    18,
    31,
    10,
    2,
    8,
    24,
    14,
    32,
    27,
    3,
    9,
    19,
    13,
    30,
    6,
    22,
    11,
    4,
    25,
};

static const int INITIAL_PERMUTATION_TABLE[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7};

static const int FINAL_PERMUTATION_TABLE[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25};

static const int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4};

static const int PC2[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32};

static const int LEFT_SHIFTS[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1};

uint32_t feistel(uint32_t R, uint64_t K) {
    // Expansion E to 48 bits
    uint64_t expandedR = 0;
    for (int i = 0; i < 48; i++) {
        expandedR = (expandedR << 1) | ((R >> (32 - EXPANSION_TABLE[i])) & 1);
    }

    // XOR with the subkey
    expandedR ^= K;

    // S-box substitution
    uint32_t output = 0;
    for (int i = 0; i < 8; i++) {
        int row = ((expandedR >> (6 * (7 - i))) & 0x20) | ((expandedR >> (6 * (7 - i) + 5)) & 1);
        int col = (expandedR >> (6 * (7 - i) + 1)) & 0xF;
        output <<= 4;
        output |= S_BOX[i][row][col];
    }

    // P-box permutation
    uint32_t final = 0;
    for (int i = 0; i < 32; i++) {
        final = (final << 1) | ((output >> (32 - PERMUTATION_TABLE[i])) & 1);
    }

    return final;
}

void des_key_setup(const uint8_t key[8], struct des_ctx *ctx)
{
    uint64_t key_reg = 0;
    // Copy the key into key_reg, ignoring parity bits
    for (int i = 0; i < 8; i++)
    {
        key_reg <<= 8;
        key_reg |= key[i];
    }

    // Apply PC-1 to the key
    uint64_t permuted_key = 0;
    for (int i = 0; i < 56; i++)
    {
        permuted_key <<= 1;
        permuted_key |= (key_reg >> (64 - PC1[i])) & 0x1;
    }

    uint32_t C = (permuted_key >> 28) & 0x0FFFFFFF; // Left 28 bits
    uint32_t D = permuted_key & 0x0FFFFFFF;         // Right 28 bits

    // Generate the 16 subkeys
    for (int round = 0; round < 16; round++)
    {
        // Left rotations
        C = ((C << LEFT_SHIFTS[round]) | (C >> (28 - LEFT_SHIFTS[round]))) & 0x0FFFFFFF;
        D = ((D << LEFT_SHIFTS[round]) | (D >> (28 - LEFT_SHIFTS[round]))) & 0x0FFFFFFF;

        // Apply PC-2 to C and D combined to get the subkeys
        uint64_t CD = ((uint64_t)C << 28) | (uint64_t)D;
        ctx->key_schedule[round] = 0;
        for (int j = 0; j < 48; j++)
        {
            ctx->key_schedule[round] <<= 1;
            ctx->key_schedule[round] |= (CD >> (56 - PC2[j])) & 0x1;
        }
    }
}

void des_encrypt(struct des_ctx *ctx, const uint8_t plaintext[8], uint8_t ciphertext[8])
{
    uint64_t block = 0;
    // Convert plaintext bytes into a single 64-bit block
    for (int i = 0; i < 8; i++)
    {
        block <<= 8;
        block |= plaintext[i];
    }

    // Apply initial permutation (IP)
    uint64_t permuted_block = 0;
    for (int i = 0; i < 64; i++)
    {
        permuted_block <<= 1;
        permuted_block |= (block >> (64 - INITIAL_PERMUTATION_TABLE[i])) & 0x1;
    }

    // Split the block into two halves
    uint32_t left = (uint32_t)(permuted_block >> 32) & 0xFFFFFFFF;
    uint32_t right = (uint32_t)(permuted_block & 0xFFFFFFFF);

    // Perform 16 rounds of the Feistel network
    for (int round = 0; round < 16; round++)
    {
        uint32_t new_right = left ^ feistel(right, ctx->key_schedule[round]);
        left = right;
        right = new_right;
    }

    // Recombine halves, switch them
    uint64_t pre_output = ((uint64_t)right << 32) | (uint64_t)left;

    // Apply final permutation (FP)
    uint64_t final_block = 0;
    for (int i = 0; i < 64; i++)
    {
        final_block <<= 1;
        final_block |= (pre_output >> (64 - FINAL_PERMUTATION_TABLE[i])) & 0x1;
    }

    // Convert the final block back into bytes for ciphertext
    for (int i = 0; i < 8; i++)
    {
        ciphertext[7 - i] = (uint8_t)(final_block & 0xFF);
        final_block >>= 8;
    }
}

// void des_decrypt(struct des_ctx *ctx, const uint8_t ciphertext[8], uint8_t plaintext[8]);

// void initial_permutation(uint8_t block[8]);

// void final_permutation(uint8_t block[8]);

void generate_subkeys(const uint64_t master_key, uint64_t subkeys[16]);

// Function to print the subkeys in byte format
// void print_subkeys(struct des_ctx *ctx) {
//     printf("DES Subkeys (in bytes):\n");
//     for (int i = 0; i < 16; i++) {
//         printf("Subkey %2d: ", i + 1);
//         for (int j = 0; j < 6; j++) { // Each subkey is 48 bits, so 6 bytes
//             printf("%02x ", (unsigned int)((ctx->key_schedule[i] >> (8 * (5 - j))) & 0xFF));
//         }
//         printf("\n");
//     }
// }

int main() {
    // Directly define key, plaintext, and expected ciphertext as hexadecimal strings
    const char* keyStr = "0E329232EA6D0D73";
    const char* plaintextStr = "8787878787878787";
    const char* expectedCiphertextStr = "0000000000000000";
    
    uint8_t key[8], plaintext[8], ciphertext[8];
    
    // Convert hexadecimal strings to byte arrays
    for (int i = 0; i < 8; i++) {
        sscanf(&keyStr[2 * i], "%2hhx", &key[i]);
        sscanf(&plaintextStr[2 * i], "%2hhx", &plaintext[i]);
    }

    struct des_ctx ctx;
    des_key_setup(key, &ctx);
    des_encrypt(&ctx, plaintext, ciphertext);

    // Prepare a string from the ciphertext for comparison
    char actualCiphertextStr[17];  // 16 characters + 1 for null-terminator
    for (int i = 0; i < 8; i++) {
        sprintf(&actualCiphertextStr[2 * i], "%02x", ciphertext[i]);
    }

    // Check if the actual ciphertext matches the expected
    if (strcmp(actualCiphertextStr, expectedCiphertextStr) == 0) {
        printf("Encryption successful: Output matches the expected ciphertext.\n");
    } else {
        printf("Encryption failed: Output does not match the expected ciphertext.\n");
        printf("Actual ciphertext: %s\n", actualCiphertextStr);
        printf("Expected  ciphertext: %s\n", actualCiphertextStr);
    }

    return 0;
}