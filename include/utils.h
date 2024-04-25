#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// Reads the entire content of a binary file into a buffer
unsigned char *read_file(const char *filename, size_t *length);

// Writes data to a binary file
void write_file(const char *filename, const unsigned char *data, size_t length);

// Generates a random key of specified size
int generate_key(unsigned char *key, size_t key_size);

#endif // UTILS_H