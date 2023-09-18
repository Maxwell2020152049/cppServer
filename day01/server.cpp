
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>  // 这个头文件包含了<inet/in.h>，可以使用IPPROTO_TCP相关的宏
#include <cstring>
#include <unistd.h>

const int N = 1024;

int main(){
    /*
        domain：选择AF_INET，表示选择IPV4协议
        type：选择SOCK_STREAM，表示套接字连接
        protocol：填0的话，自动选择协议
        IPPROTO_TCP = 6
    */
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock_fd == -1){
        std::cout << "创建socket失败" << std::endl;
        exit(1);
    }

    sockaddr_in serv_addr;

    // 设置套接字IP，协议，端口
    memset(&serv_addr, 0, sizeof serv_addr);
    
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = 4399;

    // 为套接字指定IP和端口
    bind(sock_fd, (sockaddr*)&serv_addr, sizeof serv_addr);

    // 创建监听队列
    listen(sock_fd, SOMAXCONN);

    sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof clnt_addr);

    socklen_t clnt_addr_len = sizeof clnt_addr;
    int clnt_fd = accept(sock_fd, (sockaddr* )&clnt_addr, &clnt_addr_len);

    std::cout << "client's address is " << inet_ntoa(clnt_addr.sin_addr) << ", " << 
        "client's IP protocol is " << clnt_addr.sin_family << ", " << 
        "client's Port is " << clnt_addr.sin_port << std::endl;

    char buffer[N];

    read(clnt_fd, buffer, sizeof buffer);

    std::cout << "accpet message from client: " << buffer << std::endl;

    write(clnt_fd, buffer, sizeof buffer);

    return 0;
}