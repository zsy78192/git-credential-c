[以中文阅读](README.md)

# git-credential-c

`git-credential-c` is a lightweight and secure Git credential helper written in C. It allows Git to securely store and retrieve your repository credentials (username and password) on your local machine, eliminating the need to enter them repeatedly for each operation. It uses `libsodium` for robust encryption.

## Features

*   **Secure Storage:** Encrypts and stores credentials using `libsodium`.
*   **Automatic Authentication:** Once configured, Git operations will automatically use stored credentials.
*   **Lightweight:** Written in C for minimal overhead.

## Installation

This helper is designed for RHEL-based Linux systems (like Aliyun Linux).

1.  **Clone the repository:**
    ```bash
    git clone <repository_url>
    cd git-credential-c
    ```
2.  **Run the installation script:**
    The `install.sh` script will install necessary dependencies, compile the source code, and place the executable in `/usr/local/bin`.
    ```bash
    sudo bash install.sh
    ```
    *Note: If you encounter an error regarding "Development Tools", please ensure your system's package manager can find the correct development package group. The script attempts to use `dnf groupinstall "Development Tools"`.*

## Usage

After installation, you need to configure Git to use `git-credential-c` as its credential helper.

1.  **Configure Git:**
    ```bash
    git config --global credential.helper c
    ```
    *Note: The `c` refers to the `git-credential-c` executable. Git automatically prepends `git-credential-` to the helper name.* 

2.  **First Git Operation:**
    The first time you perform a Git operation (e.g., `git pull`, `git push`) for a repository that requires authentication, you will be prompted for your username and password.
    ```bash
    git pull
    Username for 'https://example.com': your_username
    Password for 'https://your_username@example.com': your_password
    ```
    After you enter them, `git-credential-c` will securely store these credentials.

3.  **Subsequent Git Operations:**
    For all subsequent operations on the same repository, Git will automatically retrieve the stored credentials, and you will not be prompted again.

## Debugging

If you encounter issues, you can enable Git's trace output for more detailed information:

```bash
GIT_TRACE=1 git pull
```

You can also check the credential storage directory (usually `~/.git-credential-c` for the user running Git) for the `key` and `credentials` files.

## Reinstallation / Update

If you update the source code or need to reinstall, you can use the provided `reinstall.sh` script:

```bash
chmod +x reinstall.sh
sudo ./reinstall.sh
```
This script will remove old credential files, recompile, and reinstall the executable.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## My Website

You can find more of my projects and examples on my personal website: [Amazing App Examples](https://amazing.zhangzichuan.cn/app-examples.html)
