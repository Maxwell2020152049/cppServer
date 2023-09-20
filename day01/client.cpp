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
    serv_addr.sin_port = htons(4399);

    // 客户端不需要bind操作
    // bind(sock_fd, (sockaddr*)&serv_addr, sizeof serv_addr);

    int clnt_fd = connect(sock_fd, (sockaddr*)&serv_addr, sizeof serv_addr);

    char buffer[N];

    std::cin >> buffer;

    // 向服务器端打印一个字符串
    // 再从服务器端读取一个字符串
    write(sock_fd, buffer, sizeof buffer);

    read(sock_fd, buffer, sizeof buffer);

    std::cout << "accpet message from server: " << buffer << std::endl;

    return 0;
}