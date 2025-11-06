#ifndef SECURE_STORAGE_H
#define SECURE_STORAGE_H

#include "git_protocol.h"
#include <stddef.h>

// Max size for encrypted data (e.g., password) + overhead
#define ENCRYPTED_DATA_MAX_LEN (MAX_FIELD_LEN + 16 + 16) // password + crypto_secretbox_MACBYTES + crypto_secretbox_NONCEBYTES

// Structure to store an encrypted credential entry
typedef struct {
    char protocol[MAX_FIELD_LEN];
    char host[MAX_FIELD_LEN];
    char username[MAX_FIELD_LEN];
    unsigned char encrypted_password[ENCRYPTED_DATA_MAX_LEN];
    size_t encrypted_password_len;
    unsigned char nonce[24]; // crypto_secretbox_NONCEBYTES
} encrypted_credential_entry;

// Initialize the secure storage (e.g., generate/load encryption key)
int secure_storage_init();

// Save a credential (encrypt password and store)
int save_credential(const credential *cred);

// Load a credential (find, decrypt password)
// Returns 0 on success, -1 if not found or error
int load_credential(credential *cred);

// Erase a credential
int erase_credential(const credential *cred);

#endif // SECURE_STORAGE_H
