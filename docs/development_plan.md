# Development Plan

This document outlines the development plan for the `git-credential-c` project.

## Technology Stack

*   **Language**: C (C11 standard)
*   **Build System**: `Makefile`
*   **Core Libraries**:
    *   Standard C libraries (stdio, stdlib, string)
    *   `libsodium` for credential encryption and decryption.
*   **Storage Mechanism**: Encrypted credentials will be stored in a file within the user's home directory (e.g., `~/.git-credential-c/credentials`).

## Development Steps

### Step 1: Project Structure Initialization
- [x] Create project root directory.
- [x] Create `src` directory for source code (`.c` files).
- [x] Create `include` directory for header files (`.h` files).
- [x] Create `docs` directory for documentation.
- [x] Create `Makefile` for compiling and linking the project.
- [x] Create `.gitignore` file to exclude build artifacts.

### Step 2: Implement Git Credential Protocol Module
- [x] Create `git_protocol.h` and `git_protocol.c`.
- [x] Implement `parse_credential_input` to parse key-value data from Git's `stdin`.
- [x] Implement `write_credential_output` to format and write key-value data to `stdout`.

### Step 3: Implement Secure Storage Module
- [x] Create `secure_storage.h` and `secure_storage.c`.
- [ ] This module will use the `libsodium` library.
- [ ] Implement functions:
    - `secure_storage_init`: Initialize security keys (create if non-existent).
    - `save_credential`: Encrypt and save a credential to the storage file.
    - `load_credential`: Find, decrypt, and return a matching credential.
    - `erase_credential`: Remove a specified credential from storage.

### Step 4: Implement Main Application Logic
- [ ] Create `main.c`.
- [ ] The `main` function will parse command-line arguments (`get`, `store`, `erase`).
- [ ] It will orchestrate calls to the protocol and storage modules based on the command.

### Step 5: Build and Integration
- [ ] Finalize the `Makefile` to ensure all modules compile and link correctly with `libsodium`.
- [ ] Produce the final `git-credential-c` executable.
