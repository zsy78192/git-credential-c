#!/bin/bash

# This script automates the recompilation and reinstallation of git-credential-c.

# Exit immediately if a command exits with a non-zero status.
set -e

echo ">>> Step 1: Removing old credential files..."
# The credential path is determined by the HOME environment variable in the C source.
# This script assumes it is being run by the same user who runs git (in this case, root).
CRED_DIR="$HOME/.git-credential-c"
if [ -d "$CRED_DIR" ]; then
    rm -rf "$CRED_DIR"
    echo "Removed $CRED_DIR"
fi

echo ">>> Step 2: Recompiling the source code..."
make

echo ">>> Step 3: Installing the new executable..."
# This path should match the INSTALL_PATH from the original install.sh
mv -f git-credential-c /usr/local/bin/

echo ">>> Step 4: Cleaning up build artifacts..."
make clean

echo
echo "========================================================================="
echo " Reinstallation Complete!"
echo " 'git-credential-c' has been updated."
echo "========================================================================="
echo
