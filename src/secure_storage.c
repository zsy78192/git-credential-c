#include "secure_storage.h"
#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define CONFIG_DIR "/.git-credential-c"
#define KEY_FILE "/key"
#define CREDS_FILE "/credentials"

static unsigned char key[crypto_secretbox_KEYBYTES];
static char config_path[MAX_FIELD_LEN];
static char key_path[MAX_FIELD_LEN];
static char creds_path[MAX_FIELD_LEN];
static int initialized = 0;

// Helper to get the full path for config files
static int setup_paths() {
    const char *home_dir = getenv("HOME");
    if (!home_dir) {
        fprintf(stderr, "Error: HOME environment variable not set.\n");
        return -1;
    }

    snprintf(config_path, sizeof(config_path), "%s%s", home_dir, CONFIG_DIR);
    snprintf(key_path, sizeof(key_path), "%s%s", config_path, KEY_FILE);
    snprintf(creds_path, sizeof(creds_path), "%s%s", config_path, CREDS_FILE);

    return 0;
}

// Load the encryption key from file, or create it if it doesn't exist.
static int load_or_create_key() {
    struct stat st = {0};
    if (stat(config_path, &st) == -1) {
        if (mkdir(config_path, 0700) != 0 && errno != EEXIST) {
            perror("Failed to create config directory");
            return -1;
        }
    }

    FILE *fp = fopen(key_path, "rb");
    if (fp) {
        if (fread(key, 1, sizeof(key), fp) != sizeof(key)) {
            fprintf(stderr, "Error: Could not read key file.\n");
            fclose(fp);
            return -1;
        }
        fclose(fp);
    } else {
        fp = fopen(key_path, "wb");
        if (!fp) {
            perror("Failed to create key file");
            return -1;
        }
        crypto_secretbox_keygen(key);
        if (fwrite(key, 1, sizeof(key), fp) != sizeof(key)) {
            fprintf(stderr, "Error: Could not write to key file.\n");
            fclose(fp);
            return -1;
        }
        fclose(fp);
        if (chmod(key_path, 0600) != 0) {
            perror("Failed to set key file permissions");
            return -1;
        }
    }
    return 0;
}

int secure_storage_init() {
    if (initialized) return 0;

    if (sodium_init() < 0) {
        fprintf(stderr, "Error: Failed to initialize libsodium.\n");
        return -1;
    }
    if (setup_paths() != 0) {
        return -1;
    }
    if (load_or_create_key() != 0) {
        return -1;
    }

    initialized = 1;
    return 0;
}

// For now, we will implement save. Load and erase will be complex and will follow.
int save_credential(const credential *cred) {
    if (!initialized) secure_storage_init();

    encrypted_credential_entry entry;
    memset(&entry, 0, sizeof(entry));

    strncpy(entry.protocol, cred->protocol, MAX_FIELD_LEN - 1);
    strncpy(entry.host, cred->host, MAX_FIELD_LEN - 1);
    strncpy(entry.username, cred->username, MAX_FIELD_LEN - 1);

    randombytes_buf(entry.nonce, sizeof(entry.nonce));

    unsigned long long encrypted_len;
    crypto_secretbox_easy(entry.encrypted_password, (const unsigned char *)cred->password, strlen(cred->password), entry.nonce, key);
    entry.encrypted_password_len = crypto_secretbox_MACBYTES + strlen(cred->password);

    // NOTE: This is a simplified save. It just appends.
    // A real implementation should replace existing entries.
    FILE *fp = fopen(creds_path, "ab");
    if (!fp) {
        perror("Failed to open credentials file for writing");
        return -1;
    }

    fwrite(&entry, sizeof(encrypted_credential_entry), 1, fp);
    fclose(fp);

    return 0;
}

// Placeholder for load_credential
int load_credential(credential *cred) {
    if (!initialized) secure_storage_init();

    FILE *fp = fopen(creds_path, "rb");
    if (!fp) {
        // If the file doesn't exist, it's not an error, just no credential found.
        return -1; 
    }

    encrypted_credential_entry entry;
    while (fread(&entry, sizeof(encrypted_credential_entry), 1, fp) == 1) {
        if (strcmp(entry.protocol, cred->protocol) == 0 &&
            strcmp(entry.host, cred->host) == 0 &&
            strcmp(entry.username, cred->username) == 0) {
            
            unsigned char decrypted_password[MAX_FIELD_LEN];
            if (crypto_secretbox_open_easy(decrypted_password, entry.encrypted_password, entry.encrypted_password_len, entry.nonce, key) != 0) {
                fprintf(stderr, "Error: Failed to decrypt password. Credential file might be corrupt.\n");
                fclose(fp);
                return -1;
            }
            decrypted_password[entry.encrypted_password_len - crypto_secretbox_MACBYTES] = '\0';
            strncpy(cred->password, (char*)decrypted_password, MAX_FIELD_LEN - 1);
            
            fclose(fp);
            return 0; // Found and decrypted
        }
    }

    fclose(fp);
    return -1; // Not found
}

// Placeholder for erase_credential
int erase_credential(const credential *cred) {
    if (!initialized) secure_storage_init();

    // This is complex: requires reading all entries into memory (or a temp file),
    // skipping the one to erase, and writing the rest back.
    FILE *fp_in = fopen(creds_path, "rb");
    if (!fp_in) return 0; // File doesn't exist, nothing to erase.

    char temp_path[MAX_FIELD_LEN];
    snprintf(temp_path, sizeof(temp_path), "%s.tmp", creds_path);

    FILE *fp_out = fopen(temp_path, "wb");
    if (!fp_out) {
        perror("Failed to create temp file for erase");
        fclose(fp_in);
        return -1;
    }

    encrypted_credential_entry entry;
    int found = 0;
    while (fread(&entry, sizeof(encrypted_credential_entry), 1, fp_in) == 1) {
        if (!found && 
            strcmp(entry.protocol, cred->protocol) == 0 &&
            strcmp(entry.host, cred->host) == 0 &&
            strcmp(entry.username, cred->username) == 0) {
            found = 1; // Skip writing this entry
        } else {
            fwrite(&entry, sizeof(encrypted_credential_entry), 1, fp_out);
        }
    }

    fclose(fp_in);
    fclose(fp_out);

    if (rename(temp_path, creds_path) != 0) {
        perror("Failed to replace credentials file");
        remove(temp_path);
        return -1;
    }

    return 0;
}
