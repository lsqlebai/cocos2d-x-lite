此目录下的库为引擎依赖的openssl库
Android：
1.将openssl-android/openssl目录拷贝到NDK_ROOT\platforms\android-18\arch-arm\usr\include下（其中android-18取决于你编译时设置的platform版本号）

2.将openssl-android/libcrypto.so与openssl-android/libssl.so拷贝到NDK_ROOT\platforms\android-18\arch-arm\usr\lib下


Win：
1.安装openssl-1.0.2k_win32.tar.gz
2.配置环境变量OPENSSL_ROOT，
例如安装到D:\OpenSSL-Win32,则将环境变量的值设置为D:\OpenSSL-Win32