# microservice
## 基于acl 开发的微服务框架

## acl-micorservice 是基于acl框架设计的一套微服务框架。

## http_rpc rpc框架
## publisher 二进制包 发布服务。
## nameservice 服务注册
## gateservice api网关
## mqservice 消息队列服务
## configservice 配置服务
## logservice log服务
## authservice 认证服务
## msgservice 消息推送服务
## monitorservice 服务监控


                                      |------------>logic_sever----|
                                      |------------>logic_sever----|
    PC-------------->|                |------------>logic_sever----|
                     |->gateserver *N |------------>logic_sever----|
    android/ios----->|                |------------>logic_sever----|
                                      |------------>logic_sever----|
                                                                   | 
              |--------------|----------|-------------|------------| 
              |              |          |             |
           nameserver    logservice   authserver     mqserver


## 问：客户端如何访问这些服务？
答：客户端通过gateservice 服务访问 后端的服务

## 问：服务之间如何通信？
答：服务之间通过http 通讯

## 问：这么多服务，怎么找?
答：通过nameserver 定位服务

## 问：服务挂了怎么办？ 
答：当部署集群的时候，框架会尝试访问其他的服务器，
    直到有一个可以正常访问，或者都失败。


