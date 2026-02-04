# 使用Ubuntu作为基础镜像
FROM ubuntu:22.04

# 设置工作目录
WORKDIR /app

# 避免交互式提示
ENV DEBIAN_FRONTEND=noninteractive

# 安装编译工具和依赖
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    cmake \
    git \
    libsqlite3-dev \
    libpthread-stubs0-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# 复制服务器源代码
COPY . /app/

# 检查并克隆/准备第三方依赖
RUN mkdir -p thirdparty && cd thirdparty && \
    # SQLiteCpp
    if [ ! -d "SQLiteCpp" ]; then \
        git clone https://github.com/SRombauts/SQLiteCpp.git && \
        cd SQLiteCpp && \
        mkdir -p build && cd build && \
        cmake .. && make && make install && \
        cd ../..; \
    fi && \
    # nlohmann json
    if [ ! -d "json-develop" ]; then \
        git clone https://github.com/nlohmann/json.git json-develop && \
        cd json-develop && \
        mkdir -p build && cd build && \
        cmake .. && make && make install && \
        cd ../..; \
    fi && \
    cd ..

# 使用CMake构建服务器
RUN mkdir -p build && cd build && \
    cmake .. && \
    make && \
    cp server /app/server && \
    cd .. && rm -rf build

# 暴露默认端口
EXPOSE 8888

# 创建数据目录（用于SQLite数据库）
RUN mkdir -p /app/data

# 设置数据库存储路径
VOLUME ["/app/data"]

# 启动服务器
CMD ["./server", "-p", "8888"]
