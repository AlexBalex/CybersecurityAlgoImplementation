#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

unsigned char *read_file(const char *filename, size_t *length) {
    char path[1024]; // Allocate enough space for the path
    snprintf(path, sizeof(path), "../IO/%s", filename); // Prepend the directory name to the filename

    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("Failed to open file w");
        return NULL;
    }
    // Seek to the end of the file to determine the file size
    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the file content
    unsigned char *data = (unsigned char *)malloc(*length);
    if (!data) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Read the content of the file into the buffer
    if (fread(data, 1, *length, file) != *length) {
        perror("Failed to read file");
        free(data);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return data;
}

void write_file(const char *filename, const unsigned char *data, size_t length) {
    char path[1024]; // Allocate enough space for the path
    snprintf(path, sizeof(path), "../IO/%s", filename); // Prepend the directory name to the filename

    FILE *file = fopen(path, "wb");
    if (!file) {
        perror("Failed to open file w");
        return;
    }

    if (fwrite(data, 1, length, file) != length) {
        perror("Failed to write file");
    }

    fclose(file);
}

int generate_key(unsigned char *key, size_t key_size) {
    // int random_fd = open("/dev/urandom", O_RDONLY);
    // if (random_fd < 0) {
    //     perror("Failed to open /dev/urandom");
    //     return 0;
    // }

    // ssize_t result = read(random_fd, key, key_size);
    // if (result < 0) {
    //     perror("Failed to read from /dev/urandom");
    //     close(random_fd);
    //     return 0;
    // }
    // close(random_fd);

    for (size_t i = 0; i < key_size; i++) {
           key[i] = 'm';
    }
    key[key_size] = '\0';


    return 1;
}
