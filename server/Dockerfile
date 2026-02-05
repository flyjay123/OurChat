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

# 配置Git以解决网络问题
RUN git config --global http.postBuffer 524288000 && \
    git config --global http.version HTTP/1.1 && \
    git config --global http.lowSpeedLimit 0 && \
    git config --global http.lowSpeedTime 999999

# 复制服务器源代码
COPY . /app/

# 检查并克隆/准备第三方依赖
RUN mkdir -p thirdparty && cd thirdparty && \
    # SQLiteCpp - 使用浅克隆加速
    if [ ! -d "SQLiteCpp" ]; then \
        git clone --depth 1 https://github.com/SRombauts/SQLiteCpp.git && \
        cd SQLiteCpp && \
        mkdir -p build && cd build && \
        cmake .. && make -j$(nproc) && make install && \
        cd ../..; \
    fi && \
    # nlohmann json - 使用浅克隆加速
    if [ ! -d "json-develop" ]; then \
        git clone --depth 1 --branch v3.11.3 https://github.com/nlohmann/json.git json-develop && \
        cd json-develop && \
        mkdir -p build && cd build && \
        cmake .. && make -j$(nproc) && make install && \
        cd ../..; \
    fi && \
    cd ..

# 使用CMake构建服务器
RUN mkdir -p build && cd build && \
    cmake .. && \
    make -j$(nproc) && \
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
