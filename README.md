```text
      ___                 _______  __ __
     / _ \___ __ _  ___  / __/ _ \/ //_/
    / // / -_)  ' \/ _ \_\ \/ // / ,<   
   /____/\__/_/_/_/\___/___/____/_/|_|  
```

<p align="lift">
    <a href="#Language" alt="Language"><img src="https://img.shields.io/badge/language-cmake%20%7C%20shell%20%7C%20c%2Fc%2B%2B-blue" /></a>
    <a href="#arch" alt="arch"><img src="https://img.shields.io/badge/arch-arm%20-important" /></a>
    <a href="#platform" alt="platform"><img src="https://img.shields.io/badge/platform-linux-blueviolet" /></a>
</p>

# 索引

[📑 1. 项目简介](#项目简介)

[📒 2. 目录结构](#目录结构)

[💻 3. 开发环境](#开发环境)

[🔨 4. 编译代码](#编译代码)

[💯 5. 单元测试](#单元测试)

[✅ 6. 完成情况](#完成情况)

[🖥️ 7. 开发指南](#开发指南)

***

## 项目简介
这是一个用于 xxx 的 <font color = green> Linux C++ SDK</font>，主要用来 xxx。

## 目录结构

```bash
.
├── deps       # 第三方库
├── example    # 示例
├── include    # 头文件
├── src        # 源代码
└── test       # 单元测试
```

## 开发环境
+ ubuntu20.04 +
+ gcc 8.0 +
+ cmake 3.10 + 

```bash
sudo apt update
sudo apt install gcc g++ cmake make xsltproc cppcheck
```

## 编译代码
编译前先创建一个 build 目录，并生成 makefile，操作如下（在项目根目录执行）:
```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
# 或者
cmake -DCMAKE_BUILD_TYPE=Debug ..
# 如果是使用交叉编译器，请在 cmake 文件里配置并指定配置
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_PATH=../cmake/ToolChain.cmake ..
```
+ 编译示例
```bash
# 编译示例
make example
# 查看编译生成的示例
ls bin
```
+ 编译并打包 sdk
```bash
# 编译 SDK
make pack
# 查看编译生成的 SDK 包
ls install
```
+ 编译单元测试
```bash
# 编译单元测试
make test
# 查看编译生成的单元测试
ls test
```

## 单元测试
本项目采用 doctest 实现单元测试；doctest 是一个超轻量级的单元测试框架；可以实现多个测试套件（测试用例集），一个测试套件可以包含多个用例，测试时可以灵活控制要测试哪些用例或者哪些套件；用法如下：

+ 查看所有用法
```bash
./test/test_xdemo_sdk --help
```

+ 列出所有套件
```bash
./test/test_xdemo_sdk -lts
```

+ 列出所有用例
```bash
./test/test_xdemo_sdk -ltc
```

+ 测试某个套件
```bash
# 复制列出来的测试套件，粘贴代替 xxx 即可
./test/test_xdemo_sdk -lts=xxx
```

+ 测试某个用例
```bash
# 复制列出来的测试用例，粘贴代替 xxx 即可
./test/test_xdemo_sdk -ltc=xxx
```

+ 测试所有用例
```bash
./test/test_xdemo_sdk
```

## 完成情况

+ [X] 初始化
+ [X] 开始
+ [X] 停止
+ [ ] 设置回调

## 开发指南
[wiki](https://github.com/cseek/xdemo_sdk/wiki)

***
👩‍💻 <font color = red> 问题反馈: </font> jassimxiong@gmail.com

<p align="lift">Copyright (c) 2025 by Aurson, All Rights Reserved.<p>
