#include "../include/des.h"
#include <string.h>
#include <stdio.h>
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

static void key_schedule(const unsigned char *master_key, unsigned char round_keys[16][6]);
static void initial_permutation(unsigned char *block);
static void final_permutation(unsigned char *block);

// Encrypts a single block (64 bits) of plaintext
void des_encrypt(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key)
{
    unsigned char block[8];          // DES operates on 64 bits = 8 bytes
    unsigned char round_keys[16][6]; // There are 16 rounds, each needs a 48-bit key

    // Copy plaintext to block for processing
    for (int i = 0; i < 8; i++)
    {
        block[i] = plaintext[i];
    }

    // Generate all round keys
    key_schedule(key, round_keys);

    // Initial permutation
    initial_permutation(block);

    // Main DES rounds
    for (int round = 0; round < 16; round++)
    {
        // Perform the DES round processing here
        des_round(block, round_keys[round]);
    }

    // Final permutation
    final_permutation(block);

    // Copy the processed block back to ciphertext
    for (int i = 0; i < 8; i++)
    {
        ciphertext[i] = block[i];
    }
}

// Decrypts a single block (64 bits) of ciphertext
void des_decrypt(const unsigned char *ciphertext, unsigned char *plaintext, const unsigned char *key)
{
    unsigned char block[8];
    unsigned char round_keys[16][6];

    // Copy ciphertext to block for processing
    for (int i = 0; i < 8; i++)
    {
        block[i] = ciphertext[i];
    }

    // Generate all round keys
    key_schedule(key, round_keys);

    // Initial permutation
    initial_permutation(block);

    // Main DES rounds, but in reverse order for decryption
    for (int round = 15; round >= 0; round--)
    {
        // Perform the DES round processing in reverse order here
        des_round(block, round_keys[round]);
    }

    // Final permutation
    final_permutation(block);

    // Copy the processed block back to plaintext
    for (int i = 0; i < 8; i++)
    {
        plaintext[i] = block[i];
    }
}

static void initial_permutation(unsigned char *block)
{
    unsigned char new_block[8] = {0};
    for (int i = 0; i < 64; i++)
    {
        int position = INITIAL_PERMUTATION_TABLE[i] - 1;
        int value = (block[position / 8] >> (7 - (position % 8))) & 1;
        new_block[i / 8] |= value << (7 - (i % 8));
    }
    memcpy(block, new_block, 8);
}

static void final_permutation(unsigned char *block)
{
    unsigned char new_block[8] = {0};
    for (int i = 0; i < 64; i++)
    {
        int position = FINAL_PERMUTATION_TABLE[i] - 1;
        int value = (block[position / 8] >> (7 - (position % 8))) & 1;
        new_block[i / 8] |= value << (7 - (i % 8));
    }
    memcpy(block, new_block, 8);
}

void key_schedule(const unsigned char *master_key, unsigned char round_keys[16][6])
{
    unsigned char key[7] = {0}; // 56 bits

    // Apply PC-1 to get the 56-bit key from the 64-bit master key
    for (int i = 0; i < 56; i++)
    {
        int bit = PC1[i] - 1;
        if (master_key[bit / 8] & (1 << (7 - (bit % 8))))
        {
            key[i / 8] |= (1 << (7 - (i % 8)));
        }
    }

    unsigned int C = 0, D = 0; // 28 bits each

    // Load bits into C and D
    for (int i = 0; i < 28; i++)
    {
        if (key[i / 8] & (1 << (7 - (i % 8))))
        {
            C |= (1 << (27 - i));
        }
        if (key[(i + 28) / 8] & (1 << (7 - ((i + 28) % 8))))
        {
            D |= (1 << (27 - i));
        }
    }

    // Generate each of the 16 subkeys
    for (int round = 0; round < 16; round++)
    {
        // Left shifts
        C = ((C << LEFT_SHIFTS[round]) | (C >> (28 - LEFT_SHIFTS[round]))) & 0x0FFFFFFF;
        D = ((D << LEFT_SHIFTS[round]) | (D >> (28 - LEFT_SHIFTS[round]))) & 0x0FFFFFFF;

        // Apply PC-2
        unsigned char round_key[6] = {0}; // 48 bits
        unsigned int CD = (C << 28) | D;  // Concatenate C and D
        for (int i = 0; i < 48; i++)
        {
            if (CD & (1L << (56 - PC2[i])))
            {
                round_key[i / 8] |= (1 << (7 - (i % 8)));
            }
        }

        memcpy(round_keys[round], round_key, 6); // Store the round key
    }
}

void des_round(unsigned char *block, const unsigned char *round_key)
{
    unsigned char expanded_block[6] = {0}; // 48 bits
    // Expansion step
    for (int i = 0; i < 48; i++)
    {
        int bit_position = EXPANSION_TABLE[i] - 1;
        expanded_block[i / 8] |= ((block[bit_position / 8] >> (7 - (bit_position % 8))) & 1) << (7 - (i % 8));
    }

    // XOR with the round key
    for (int i = 0; i < 6; i++)
    {
        expanded_block[i] ^= round_key[i];
    }

    // Substitution with S-boxes
    unsigned char substitution_output[4] = {0}; // 32 bits output
    for (int i = 0; i < 8; i++)
    {
        int row = ((expanded_block[(i * 6) / 8] >> (7 - ((i * 6) % 8))) & 1) << 1; // Most significant bit
        row |= (expanded_block[(i * 6 + 5) / 8] >> (7 - ((i * 6 + 5) % 8))) & 1;   // Least significant bit

        int column = 0;
        for (int j = 1; j <= 4; j++)
        { // Middle four bits
            column <<= 1;
            column |= (expanded_block[(i * 6 + j) / 8] >> (7 - ((i * 6 + j) % 8))) & 1;
        }

        unsigned char sbox_value = S_BOX[i][row][column]; // Access the correct S-box for this 6-bit segment
        for (int j = 0; j < 4; j++)
        {
            // Set each bit in the output array. Output is stored in a compact form, so calculations for bit positions are necessary.
            substitution_output[(i * 4) / 8] |= ((sbox_value >> (3 - j)) & 1) << (7 - ((i * 4 + j) % 8));
        }
    }

    // Permutation step
    unsigned char permuted_block[4] = {0};
    for (int i = 0; i < 32; i++)
    {
        int bit_position = PERMUTATION_TABLE[i] - 1;
        permuted_block[i / 8] |= ((substitution_output[bit_position / 8] >> (7 - (bit_position % 8))) & 1) << (7 - (i % 8));
    }

    // Combine the result with the left half of the block
    for (int i = 0; i < 4; i++)
    {
        block[i] ^= permuted_block[i];
    }
}
