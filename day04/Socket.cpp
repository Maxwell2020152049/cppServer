// Copyright [2023] <Wanfeng Huang>

#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "day04/err.h"
#include "day04/Socket.h"
#include "Socket.h"

/*
    domain：选择AF_INET，表示选择IPV4协议
    type：选择SOCK_STREAM，表示套接字连接
    protocol：填0的话，自动选择协议
    IPPROTO_TCP = 6
*/
Socket::Socket(int domain, int type, int protocol) {
    handle_error((this->fd = socket(domain, type, protocol)) == -1,
        "创建socket失败");
}

Socket::~Socket() {
    handle_error(close(this->fd) == -1, "关闭socket失败");
}

int Socket::get_fd() {
    return this->fd;
}

void Socket::sock_bind(in_addr_t ip, sa_family_t family, in_port_t port) {
    // 设置套接字IP，协议，端口
    struct sockaddr_in serv_addr;

    serv_addr.sin_addr.s_addr = ip;
    serv_addr.sin_family = family;
    serv_addr.sin_port = port;

    // 为套接字指定IP和端口
    handle_error(
        bind(this->fd,
            reinterpret_cast<sockaddr*>(&serv_addr),
            sizeof serv_addr) == -1,
        "bind失败");
}

void Socket::sock_listen(int n) {
    // 创建监听队列
    handle_error(listen(this->fd, n) == -1, "listen失败");
}

