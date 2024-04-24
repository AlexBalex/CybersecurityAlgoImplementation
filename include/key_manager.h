#ifndef KEY_MANAGER_H
#define KEY_MANAGER_H

#include <stddef.h>

// Saves the encryption key to a file
int save_key(const unsigned char *key, size_t key_size, const char *key_file_path);

// Loads the encryption key from a file
int load_key(unsigned char *key, size_t key_size, const char *key_file_path);

#endif // KEY_MANAGER_H