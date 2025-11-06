#ifndef GIT_PROTOCOL_H
#define GIT_PROTOCOL_H

#define MAX_FIELD_LEN 256

typedef struct {
    char protocol[MAX_FIELD_LEN];
    char host[MAX_FIELD_LEN];
    char username[MAX_FIELD_LEN];
    char password[MAX_FIELD_LEN];
    char path[MAX_FIELD_LEN];
    char url[MAX_FIELD_LEN];
    char port[MAX_FIELD_LEN];
    char *other_fields; // For any other fields Git might send
} credential;

// Function to parse credential information from stdin
void parse_credential_input(credential *cred);

// Function to write credential information to stdout
void write_credential_output(const credential *cred);

#endif // GIT_PROTOCOL_H
