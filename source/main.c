#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "../include/des.h"
#include "../include/aes.h"
#include "../include/rsa.h"
#include "../include/utils.h"
#include "../include/key_manager.h"
#define DES_KEY_SIZE 8
#define DES_BLOCK_SIZE 8
#define AES128_KEY_SIZE 16
#define AES192_KEY_SIZE 24
#define AES256_KEY_SIZE 32
#define AES_BLOCK_SIZE 16
#define AES128_ROUNDS 10
#define AES192_ROUNDS 12
#define AES256_ROUNDS 14

void process_data(const unsigned char *input_data, size_t input_length,
                  const char *output_filename, const unsigned char *key,
                  char mode, const char *algorithm, int keySize, int rounds, RSAKey *rsaKey)
{
    // Determine block size based on the algorithm
    size_t block_size = (strcmp(algorithm, "aes") == 0) ? AES_BLOCK_SIZE : 
                      (strcmp(algorithm, "des") == 0) ? DES_BLOCK_SIZE : 1;
    size_t output_length = ((input_length + block_size - 1) / block_size) * block_size;
    unsigned char *output_data = malloc(output_length);
    if (!output_data)
    {
        perror("Failed to allocate memory for output data");
        return;
    }
    memset(output_data, 0, output_length);

    if (strcmp(algorithm, "rsa") == 0)
    {
        mpz_t m, c;
        mpz_init(m);
        mpz_init(c);
        mpz_import(m, input_length, 1, sizeof(input_data[0]), 0, 0, input_data);

        if (mode == 'e')
        {
            rsa_encrypt(c, m, rsaKey);
            size_t count = 0;
            mpz_export(output_data, &count, 1, sizeof(output_data[0]), 0, 0, c);
            output_length = count;
        }
        else
        {
            rsa_decrypt(m, c, rsaKey);
            size_t count = 0;
            mpz_export(output_data, &count, 1, sizeof(output_data[0]), 0, 0, m);
            output_length = count;
        }
        mpz_clear(m);
        mpz_clear(c);
    }
    else
    {
        size_t processed_length = 0;

        for (size_t i = 0; i < input_length; i += block_size)
        {
            size_t chunk_size = (i + block_size <= input_length) ? block_size : (input_length % block_size);
            unsigned char processed_block[block_size];
            memset(processed_block, 0, block_size); 
            memcpy(processed_block, input_data + i, chunk_size);

            if (mode == 'e')
            {
                if (chunk_size < block_size) // Apply padding
                {
                    size_t padding_size = block_size - chunk_size;
                    memset(processed_block + chunk_size, 0, padding_size);
                    chunk_size = block_size; // Adjust chunk_size to full block size after padding
                }

                if (strcmp(algorithm, "aes") == 0)
                {
                    AES_Encrypt(processed_block, key, output_data + processed_length, keySize, rounds);
                }
                else if (strcmp(algorithm, "des") == 0)
                {
                    des_encrypt(processed_block, output_data + processed_length, key);
                }
                processed_length += block_size;
            }
            else if (mode == 'd')
            {
                if (strcmp(algorithm, "aes") == 0)
                {
                    AES_Decrypt(processed_block, key, output_data + processed_length, keySize, rounds);
                }
                else if (strcmp(algorithm, "des") == 0)
                {
                    des_decrypt(processed_block, output_data + processed_length, key);
                }

                processed_length += block_size;

                // Remove padding after the last block processed
                if (i + block_size >= input_length)
                {
                    size_t padding_size = output_data[processed_length - 1];
                    if (padding_size <= block_size && padding_size > 0 && padding_size <= block_size)
                    {
                        processed_length -= padding_size; // Adjust length to remove padding
                    }
                }
            }
        }
        output_length = processed_length;
    }

    // Write the processed data to the output file
    write_file(output_filename, output_data, output_length);
    free(output_data);
}


int main(int argc, char **argv)
{
    if (argc != 6)
    {
        fprintf(stderr, "Usage: %s <inputfile> <outputfile> <mode> <algorithm> <keyfile>\n", argv[0]);
        return 1;
    }
    const char *input_filename = argv[1];
    const char *output_filename = argv[2];
    char mode = argv[3][0]; // 'e' for encrypt, 'd' for decrypt
    const char *algorithm = argv[4];
    char key_file[256];
    snprintf(key_file, sizeof(key_file), "../keys/%s", argv[5]);

    int keySize = 0;
    int aesRounds = 0;
    RSAKey keyy;
    int bit_size = 2048;

    if (strncmp(algorithm, "aes128", 6) == 0)
    {
        algorithm = "aes";
        keySize = AES128_KEY_SIZE;
        aesRounds = AES128_ROUNDS;
    }
    else if (strncmp(algorithm, "aes192", 6) == 0)
    {
        algorithm = "aes";
        keySize = AES192_KEY_SIZE;
        aesRounds = AES192_ROUNDS;
    }
    else if (strncmp(algorithm, "aes256", 6) == 0)
    {
        algorithm = "aes";
        keySize = AES256_KEY_SIZE;
        aesRounds = AES256_ROUNDS;
    }
    else if (strcmp(algorithm, "des") == 0)
    {
        algorithm = "des";
        keySize = DES_KEY_SIZE;
    }
    else if (strcmp(algorithm, "rsa") == 0)
    {
        generate_rsa_keys(&keyy, bit_size);
    }
    else
    {
        fprintf(stderr, "Unsupported algorithm: %s\n", algorithm);
        return 1;
    }

    size_t input_length;
    unsigned char *input_data = read_file(input_filename, &input_length);
    if (!input_data)
    {
        fprintf(stderr, "!Error reading input file\n");
        return 1;
    }

    unsigned char key[keySize];

    if (strcmp(algorithm, "rsa") == 0)
    {
        process_data(input_data, input_length, output_filename, NULL, mode, algorithm, keySize, aesRounds, &keyy);
    }
    else
    {
        if (strcmp(algorithm, "aes") == 0)
        {
            if (mode == 'e')
            {
                generate_key(key, keySize);
                if (save_key(key, keySize, key_file) != 0)
                {
                    fprintf(stderr, "Failed to save AES key\n");
                    free(input_data);
                    return 1;
                }
            }
            else if (mode == 'd')
            {
                if (load_key(key, keySize, key_file) != 0)
                {
                    fprintf(stderr, "Failed to load AES key\n");
                    free(input_data);
                    return 1;
                }
            }
            // Assume process_data is adapted to use aesRounds if necessary
            process_data(input_data, input_length, output_filename, key, mode, algorithm, keySize, aesRounds, NULL);
        }
        else if (strcmp(algorithm, "des") == 0)
        {
            if (mode == 'e')
            {
                generate_key(key, keySize);
                if (save_key(key, keySize, key_file) != 0)
                {
                    fprintf(stderr, "Failed to save DES key\n");
                    free(input_data);
                    return 1;
                }
            }
            else if (mode == 'd')
            {
                if (load_key(key, keySize, key_file) != 0)
                {
                    fprintf(stderr, "Failed to load DES key\n");
                    free(input_data);
                    return 1;
                }
            }
            process_data(input_data, input_length, output_filename, key, mode, algorithm, keySize, aesRounds, NULL);
        }
        else
        {
            fprintf(stderr, "Unsupported algorithm: %s\n", algorithm);
            free(input_data);
            return 1;
        }
    }
    free(input_data);
    return 0;
}