#include <stdio.h>
#include <string.h>
#include "git_protocol.h"
#include "secure_storage.h"

void show_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s <get|store|erase>\n", prog_name);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        show_usage(argv[0]);
        return 1;
    }

    if (secure_storage_init() != 0) {
        // Errors are printed within the function
        return 1;
    }

    char *command = argv[1];
    credential cred;
    parse_credential_input(&cred);

    if (strcmp(command, "store") == 0) {
        // A more robust implementation would erase before saving
        // to prevent duplicate entries.
        erase_credential(&cred); 
        if (save_credential(&cred) != 0) {
            fprintf(stderr, "Failed to save credential.\n");
            return 1;
        }
    } else if (strcmp(command, "get") == 0) {
        if (load_credential(&cred) == 0) {
            write_credential_output(&cred);
        } else {
            // If not found, git will prompt the user. This is normal.
            // No output means "not found".
        }
    } else if (strcmp(command, "erase") == 0) {
        if (erase_credential(&cred) != 0) {
            fprintf(stderr, "Failed to erase credential.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Error: Unknown command '%s'\n", command);
        show_usage(argv[0]);
        return 1;
    }

    return 0;
}
