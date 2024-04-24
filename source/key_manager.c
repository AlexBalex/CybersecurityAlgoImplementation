#include "../include/key_manager.h"
#include <stdio.h>
#include <stdlib.h>

int save_key(const unsigned char *key, size_t key_size, const char *key_file_path) {
    FILE *file = fopen(key_file_path, "wb");
    if (!file) {
        perror("Failed to open key file for writing");
        return -1;
    }
    if (fwrite(key, 1, key_size, file) != key_size) {
        perror("Failed to write key to file");
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;
}

int load_key(unsigned char *key, size_t key_size, const char *key_file_path) {
    FILE *file = fopen(key_file_path, "rb");
    if (!file) {
        perror("Failed to open key file for reading");
        return -1;
    }
    if (fread(key, 1, key_size, file) != key_size) {
        perror("Failed to read key from file");
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;
}
