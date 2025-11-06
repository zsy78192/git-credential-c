#!/bin/bash

# This script automates the installation of git-credential-c on RHEL-based systems like Aliyun OS.

# Exit immediately if a command exits with a non-zero status.
set -e

# --- Configuration ---
INSTALL_PATH="/usr/local/bin"
EXECUTABLE_NAME="git-credential-c"

# --- Helper Functions ---
check_root() {
    if [ "$(id -u)" -ne 0 ]; then
        echo "Error: This script must be run with sudo or as the root user." >&2
        exit 1
    fi
}

# --- Main Logic ---
main() {
    check_root

    echo ">>> Step 1: Installing dependencies (Development Tools, libsodium-devel, git)..."
    dnf groupinstall -y "Development Tools" && dnf install -y libsodium-devel git

    echo ">>> Step 2: Preparing Makefile for Linux environment..."
    # Remove macOS-specific Homebrew paths. A backup `Makefile.bak` will be created.
    sed -i.bak 's| -I/opt/homebrew/include||g' Makefile
    sed -i.bak 's| -L/opt/homebrew/lib||g' Makefile
    echo "Makefile has been adjusted for Linux."

    echo ">>> Step 3: Compiling the source code..."
    make

    echo ">>> Step 4: Installing executable to ${INSTALL_PATH}..."
    mv -f "${EXECUTABLE_NAME}" "${INSTALL_PATH}/"
    chmod 755 "${INSTALL_PATH}/${EXECUTABLE_NAME}"
    echo "Executable installed."

    echo ">>> Step 5: Cleaning up build artifacts..."
    make clean
    rm -f Makefile.bak # Clean up sed backup file

    echo
    echo "========================================================================"
    echo " Installation Complete!"
    echo " 'git-credential-c' is now available in your system path."
    echo "========================================================================"
    echo
    echo " IMPORTANT: To complete the setup, please run the following command"
    echo "            using your regular user account (NOT as root):"
    echo
    echo "   git config --global credential.helper git-credential-c"
    echo
}

# --- Run Script ---
main
