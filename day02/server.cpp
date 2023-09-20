
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>  // 这个头文件包含了<inet/in.h>，可以使用IPPROTO_TCP相关的宏
#include <cstring>
#include <unistd.h>
#include "err.h"

const int BUF_SIZE = 1024;

int main(){
    /*
        domain：选择AF_INET，表示选择IPV4协议
        type：选择SOCK_STREAM，表示套接字连接
        protocol：填0的话，自动选择协议
        IPPROTO_TCP = 6
    */
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    handle_error(sock_fd == -1, "创建socket失败");

    sockaddr_in serv_addr;

    // 设置套接字IP，协议，端口
    memset(&serv_addr, 0, sizeof serv_addr);
    
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4399);

    // 为套接字指定IP和端口
    handle_error(bind(sock_fd, (sockaddr*)&serv_addr, sizeof serv_addr) == -1, "bind失败");

    // 创建监听队列
    handle_error(listen(sock_fd, SOMAXCONN) == -1, "listen失败");

    sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof clnt_addr);

    //  等待客户端连接
    socklen_t clnt_addr_len = sizeof clnt_addr;
    int clnt_fd = accept(sock_fd, (sockaddr* )&clnt_addr, &clnt_addr_len);
    handle_error(sock_fd == -1, "accept 失败");

    // 打印连接服务器的客户端的信息
    std::cout << "client's address is " << inet_ntoa(clnt_addr.sin_addr) << ", " << 
        "client's IP protocol is " << clnt_addr.sin_family << ", " << 
        "client's Port is " << clnt_addr.sin_port << std::endl;

    // 从客户端接收一个字符串
    // 再向客户端打印一个字符串
    while (true){
        char buffer[BUF_SIZE];

        memset(buffer, 0, sizeof buffer);

        handle_error(read(clnt_fd, buffer, sizeof buffer) == -1, "read失败");

        if (!strcmp(buffer, "q") || !strcmp(buffer, "Q")){
            std::cout << "client disconnected" << std::endl;
            break;
        }

        std::cout << "accpet message from client: " << buffer << std::endl;

        handle_error(write(clnt_fd, buffer, sizeof buffer) == -1, "write失败");
    }

    return 0;
}