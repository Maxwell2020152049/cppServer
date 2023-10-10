// Copyright [2023] <Wanfeng Huang>

#include <arpa/inet.h>  // 这个头文件包含了<inet/in.h>，可以使用IPPROTO_TCP相关的宏
#include <unistd.h>
#include <sys/socket.h>

#include <cstring>
#include <iostream>

#include "day04/err.h"

const int BUF_SIZE = 1024;

int main() {
    /*
        domain：选择AF_INET，表示选择IPV4协议
        type：选择SOCK_STREAM，表示套接字连接
        protocol：填0的话，自动选择协议
        IPPROTO_TCP = 6
    */
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    handle_error(sock_fd == -1, "创建socket失败");

    struct sockaddr_in serv_addr;

    // 设置套接字IP，协议，端口
    memset(&serv_addr, 0, sizeof serv_addr);

    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4399);

    // 客户端不需要bind操作
    // bind(sock_fd, (sockaddr*)&serv_addr, sizeof serv_addr);

    int clnt_fd = connect(sock_fd, reinterpret_cast<sockaddr*>(&serv_addr),
        sizeof serv_addr);

    handle_error(clnt_fd == -1, "connect失败");

    while (true) {
        char buffer[BUF_SIZE];

        memset(buffer, 0, sizeof buffer);

        std::cin >> buffer;

        // 向服务器端打印一个字符串
        // 再从服务器端读取一个字符串
        handle_error(write(sock_fd, buffer, sizeof buffer) == -1, "write失败");

        if (!strcmp(buffer, "q") || !strcmp(buffer, "Q")) {
            std::cout << "服务器断开连接" << std::endl;
            break;
        }

        handle_error(read(sock_fd, buffer, sizeof buffer) == -1, "read失败");

        std::cout << "接收来自服务器的消息: " << buffer << std::endl;
    }

    handle_error(close(sock_fd) == -1, "关闭socket失败");
    handle_error(close(clnt_fd) == -1, "关闭socket失败");

    return 0;
}
