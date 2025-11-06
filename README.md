[Read in English](README.en.md)

# git-credential-c

`git-credential-c` 是一个用 C 语言编写的轻量级、安全的 Git 凭据助手。它允许 Git 在您的本地机器上安全地存储和检索您的仓库凭据（用户名和密码），从而无需在每次操作时重复输入。它使用 `libsodium` 进行强大的加密。

## 功能特性

*   **安全存储：** 使用 `libsodium` 加密并存储凭据。
*   **自动认证：** 配置后，Git 操作将自动使用存储的凭据。
*   **轻量级：** 用 C 语言编写，开销极小。

## 安装

此助手专为基于 RHEL 的 Linux 系统（如阿里云 Linux）设计。

1.  **克隆仓库：**
    ```bash
    git clone <repository_url>
    cd git-credential-c
    ```
2.  **运行安装脚本：**
    `install.sh` 脚本将安装必要的依赖项，编译源代码，并将可执行文件放置在 `/usr/local/bin` 中。
    ```bash
    sudo bash install.sh
    ```
    *注意：如果您遇到关于“Development Tools”的错误，请确保您的系统包管理器能够找到正确的开发包组。脚本尝试使用 `dnf groupinstall "Development Tools"`。*

## 使用方法

安装完成后，您需要配置 Git 以使用 `git-credential-c` 作为其凭据助手。

1.  **配置 Git：**
    ```bash
    git config --global credential.helper c
    ```
    *注意：`c` 指的是 `git-credential-c` 可执行文件。Git 会自动在助手名称前加上 `git-credential-`。*

2.  **首次 Git 操作：**
    当您首次对需要身份验证的仓库执行 Git 操作（例如 `git pull`、`git push`）时，系统会提示您输入用户名和密码。
    ```bash
    git pull
    Username for 'https://example.com': your_username
    Password for 'https://your_username@example.com': your_password
    ```
    输入后，`git-credential-c` 将安全地存储这些凭据。

3.  **后续 Git 操作：**
    对于同一仓库的所有后续操作，Git 将自动检索存储的凭据，您将不再收到提示。

## 调试

如果遇到问题，您可以启用 Git 的跟踪输出以获取更详细的信息：

```bash
GIT_TRACE=1 git pull
```

您还可以检查凭据存储目录（对于运行 Git 的用户，通常是 `~/.git-credential-c`）中是否存在 `key` 和 `credentials` 文件。

## 重新安装 / 更新

如果您更新了源代码或需要重新安装，可以使用提供的 `reinstall.sh` 脚本：

```bash
chmod +x reinstall.sh
sudo ./reinstall.sh
```
此脚本将删除旧的凭据文件，重新编译并重新安装可执行文件。

## License (许可证)

本项目采用 MIT 许可证。详情请参阅 `LICENSE` 文件。

## My Website (我的网站)

您可以在我的个人网站上找到更多我的项目和示例：[Amazing App Examples](https://amazing.zhangzichuan.cn/app-examples.html)