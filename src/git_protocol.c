#include "git_protocol.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Helper function to set a field in the credential struct
static void set_credential_field(credential *cred, const char *key, const char *value) {
    if (strcmp(key, "protocol") == 0) {
        strncpy(cred->protocol, value, MAX_FIELD_LEN - 1);
        cred->protocol[MAX_FIELD_LEN - 1] = '\0';
    } else if (strcmp(key, "host") == 0) {
        strncpy(cred->host, value, MAX_FIELD_LEN - 1);
        cred->host[MAX_FIELD_LEN - 1] = '\0';
    } else if (strcmp(key, "username") == 0) {
        strncpy(cred->username, value, MAX_FIELD_LEN - 1);
        cred->username[MAX_FIELD_LEN - 1] = '\0';
    } else if (strcmp(key, "password") == 0) {
        strncpy(cred->password, value, MAX_FIELD_LEN - 1);
        cred->password[MAX_FIELD_LEN - 1] = '\0';
    } else if (strcmp(key, "path") == 0) {
        strncpy(cred->path, value, MAX_FIELD_LEN - 1);
        cred->path[MAX_FIELD_LEN - 1] = '\0';
    } else if (strcmp(key, "url") == 0) {
        strncpy(cred->url, value, MAX_FIELD_LEN - 1);
        cred->url[MAX_FIELD_LEN - 1] = '\0';
    } else if (strcmp(key, "port") == 0) {
        strncpy(cred->port, value, MAX_FIELD_LEN - 1);
        cred->port[MAX_FIELD_LEN - 1] = '\0';
    } else {
        // Handle other fields if necessary, or ignore
        // For simplicity, we'll ignore other fields for now or append to other_fields
    }
}

void parse_credential_input(credential *cred) {
    // Initialize credential fields to empty strings
    memset(cred, 0, sizeof(credential));

    char line[MAX_FIELD_LEN * 2]; // Buffer for reading lines
    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove trailing newline character
        line[strcspn(line, "\n")] = '\0';

        char *equals_sign = strchr(line, '=');
        if (equals_sign) {
            *equals_sign = '\0'; // Null-terminate the key
            char *key = line;
            char *value = equals_sign + 1;
            set_credential_field(cred, key, value);
        }
    }
}

void write_credential_output(const credential *cred) {
    if (cred->protocol[0] != '\0') {
        printf("protocol=%s\n", cred->protocol);
    }
    if (cred->host[0] != '\0') {
        printf("host=%s\n", cred->host);
    }
    if (cred->username[0] != '\0') {
        printf("username=%s\n", cred->username);
    }
    if (cred->password[0] != '\0') {
        printf("password=%s\n", cred->password);
    }
    if (cred->path[0] != '\0') {
        printf("path=%s\n", cred->path);
    }
    if (cred->url[0] != '\0') {
        printf("url=%s\n", cred->url);
    }
    if (cred->port[0] != '\0') {
        printf("port=%s\n", cred->port);
    }
}
