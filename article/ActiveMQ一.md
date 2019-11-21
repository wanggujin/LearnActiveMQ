# 开发环境
- Ubuntu 16.04-x64
- GCC-5.4 / CMake 3.13
- jdk 13
- ActiveMQ 5.15
- ActiveMQ-CPP 3.9.5

# 环境安装
- 安装并配置好jdk环境
- 下载[ActiveMQ安装包](http://activemq.apache.org/components/classic/download/)，然后解压即可。笔者的ActiveMQ目录是/opt/activemq
- 下载[ActiveMQ-CPP](http://activemq.apache.org/components/cms/download/)，并编译安装到自己喜欢的目录(需要安装 apache apr)。笔者将其安装在/opt/activemqcpp

# 简介
众所周知，MQ即Message Queue，翻译为消息队列，也有人称其为消息中间件。主要用于互联网企业，在大型系统中起到解耦、异步、流量削峰的作用。当前主流的消息中间件产品有很多，比如ActiveMQ、RabbitMQ、RocketMQ、Kafka、ZeroMQ等等。