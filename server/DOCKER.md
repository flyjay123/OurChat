# OurChat 服务器 Docker 部署指南

## 快速开始

### 方法1: 使用 Docker Compose（推荐）

```bash
# 构建并启动服务
docker-compose up -d

# 查看日志
docker-compose logs -f

# 停止服务
docker-compose down
```

### 方法2: 使用 Docker 命令

```bash
# 构建镜像
docker build -t ourchat-server .

# 运行容器
docker run -d \
  --name ourchat-server \
  -p 8888:8888 \
  -v $(pwd)/data:/app/data \
  ourchat-server

# 查看日志
docker logs -f ourchat-server

# 停止容器
docker stop ourchat-server

# 删除容器
docker rm ourchat-server
```

## 配置说明

### 端口配置
- 默认端口: 8888
- 修改端口: 编辑 `docker-compose.yml` 中的 `ports` 配置，或在 docker run 时使用 `-p` 参数

### 数据持久化
- 数据库文件存储在 `./data` 目录
- 容器重启后数据不会丢失

### 自定义端口启动
修改 `docker-compose.yml` 中的 CMD 部分，或直接运行：
```bash
docker run -d -p 9000:9000 -v $(pwd)/data:/app/data ourchat-server ./server -p 9000
```

## 故障排查

### 查看容器状态
```bash
docker ps -a
```

### 查看容器日志
```bash
docker logs ourchat-server
```

### 进入容器调试
```bash
docker exec -it ourchat-server /bin/bash
```

## 更新服务

```bash
# 使用 docker-compose
docker-compose down
docker-compose build --no-cache
docker-compose up -d

# 使用 docker 命令
docker stop ourchat-server
docker rm ourchat-server
docker build -t ourchat-server . --no-cache
docker run -d --name ourchat-server -p 8888:8888 -v $(pwd)/data:/app/data ourchat-server
```

## 性能优化

### 多阶段构建优化
如需更小的镜像体积，可以修改 Dockerfile 使用多阶段构建，将编译环境和运行环境分离。

### 资源限制
在生产环境中，建议限制容器资源使用：
```bash
docker run -d \
  --name ourchat-server \
  --memory="512m" \
  --cpus="1.0" \
  -p 8888:8888 \
  -v $(pwd)/data:/app/data \
  ourchat-server
```
