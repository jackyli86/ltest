
# 安装vcpkg

```
# 下载
git clone https://github.com/microsoft/vcpkg.git

# 选择稳定版本
git tags

// 安装vcpkg
bootstrap-vcpkg.bat

// 集成到Visual Studio 
.\vcpkg.exe integrate install

// 设置环境变量
VCPKG_ROOT={vcpkg_root}

// TRIPLET默认是x64-windows
// 注意当不是默认值时,需要在VS中指定TRIPLET
VCPKG_DEFAULT_TRIPLET=x64-windows-static

```

# 从清单安装

```
// 全局安装
vcpkg install --classic
vcpkg install --x-install-root={vcpkg_root}\installed 
```

