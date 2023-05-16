# OurChat

一个自制IM，采用QT6制作客户端，linux C++实现后端。
## 聊天界面
![image ](https://user-images.githubusercontent.com/118870803/236397577-fdee476e-43dc-4639-bf26-985f23f71f5b.png)

## 选择头像
![image](https://user-images.githubusercontent.com/118870803/236397873-4704059a-e0c8-4c22-a3ef-4ed00495eaf9.png)
## 发送表情
![image](https://github.com/flyjay123/OurChat/assets/118870803/e85c9496-9a64-48c6-981e-4504b3e134a3)

# 构建
## Server
需要sqlite3作为数据库底层驱动,之后编译SQLiteCpp静态库。
```bash
cd OurChat/server
mkdir build
cd build
cmake ..
make
```

## Client
使用CMake指定自己的编译器进行构建,也可以直接用Qt Creator打开.pro文件,选择MinGW或MSVC构建项目,编译之前先qmake.
本项目采用Qt6开发,使用Qt5构建需要修改部分代码.

编译前应当修改TcpClient.h中的宏定义ip和port.