# OurChat
一款基于Qt的网络聊天室，高仿wechat
<br>一个自制IM，采用QT6制作客户端，linux C++实现后端。
## 聊天界面
![513(8@KQJNHXV6U)JJ)O1_G](https://github.com/flyjay123/OurChat/assets/118870803/5324a713-b900-48fe-8b81-b85eef3d032e)

## 选择头像
![image](https://user-images.githubusercontent.com/118870803/236397873-4704059a-e0c8-4c22-a3ef-4ed00495eaf9.png)
## 发送表情
![image](https://github.com/flyjay123/OurChat/assets/118870803/e85c9496-9a64-48c6-981e-4504b3e134a3)

# 构建
## Server
- sqlite3作为数据库底层驱动和SQLiteCpp静态库。
```bash
cd OurChat/server/thirdparty
git clone git@github.com:SRombauts/SQLiteCpp.git
```
- nlohmann/json库。
```bash
cd OurChat/server/thirdparty
git clone git@github.com:nlohmann/json.git
```
现在可以开始编译server
```bash
cd OurChat/server
mkdir build
cd build
cmake ..
make
./server -s ${IpAddress} -p ${Port}
```

## Client
本项目支持cmake和qmake,采用Qt6开发,使用Qt5构建需要修改部分代码.
<br>
以MinGW作为示例：
<br>
将 "Qt\Qt6.5.0\Tools\MinGW1120_64\bin" (视Qt版本和MinGW版本而自行更正目录) 添加至环境变量
<br>
定位至CMakeLists.txt line:36, 按自己的编译器路径修改 CMAKE_PREFIX_PATH 
```bash
cd OurChat\Client
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make.exe
.\OurChat.exe
```

编译前应当修改Tools\tcpclient.h中m_server与m_port变量，以设置IP与端口号。

## Visitor Count

![:name](https://count.getloli.com/get/@flyjay123?theme=gelbooru)
