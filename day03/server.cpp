
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>  // 这个头文件包含了<inet/in.h>，可以使用IPPROTO_TCP相关的宏
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include "err.h"

const int BUF_SIZE = 1024;
const int MAX_EVENTS = 1024;

int main(){
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

    // 为套接字指定IP和端口
    handle_error(bind(sock_fd, (sockaddr*)&serv_addr, sizeof serv_addr) == -1, "bind失败");

    // 创建监听队列
    handle_error(listen(sock_fd, SOMAXCONN) == -1, "listen失败");

    // 创建epoll实例
    int epfd = epoll_create1(0);
    handle_error(epfd == -1, "创建epoll实例错误");

    // 定义epoll事件
    struct epoll_event events[MAX_EVENTS], ev;
    memset(events, 0, sizeof events);
    memset(&ev, 0, sizeof ev);

    ev.data.fd = sock_fd;
    ev.events = EPOLLIN ;

    // 设置套接字为不阻塞模式
    handle_error(fcntl(sock_fd, F_SETFL, fcntl(sock_fd, F_GETFL) | O_NONBLOCK) == -1, "设置socket状态失败");

    handle_error(epoll_ctl(epfd, EPOLL_CTL_ADD, sock_fd, &ev) == -1, "添加sock_fd到epoll失败");
    
    while (true){
        // 一直等待监听的文件描述符发生可读事件或者sock_fd收到新的连接请求
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        handle_error(nfds == -1, "epoll wait失败");

        for (int i = 0; i < nfds; i++){
            if (events[i].data.fd == sock_fd){
                // 客户端连接服务器
                struct sockaddr_in clnt_addr;
                memset(&clnt_addr, 0, sizeof clnt_addr);

                socklen_t clnt_addr_len = sizeof clnt_addr;
                int clnt_fd = accept(sock_fd, (sockaddr* )&clnt_addr, &clnt_addr_len);
                handle_error(clnt_fd == -1, "accept失败");

                // 打印连接服务器的客户端的信息
                std::cout << "client's address is " << inet_ntoa(clnt_addr.sin_addr) << ", " << 
                    "client's IP protocol is " << clnt_addr.sin_family << ", " << 
                    "client's Port is " << clnt_addr.sin_port << ", " <<
                    "client's fd is " << clnt_fd << std::endl;

                // 将客户端的文件描述符添加到epoll实例中
                memset(&ev, 0, sizeof ev);
                ev.data.fd = clnt_fd;
                ev.events = EPOLLIN | EPOLLET;
                fcntl(clnt_fd, F_SETFL, fcntl(clnt_fd, F_GETFL) | O_NONBLOCK);
                handle_error(epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_fd, &ev) == -1, "添加clnt_fd到epoll失败");
            }else if (events[i].events & EPOLLIN){
                while(true){
                    // 从客户端接收一个字符串
                    // 再向客户端打印一个字符串
                    char buffer[BUF_SIZE];

                    memset(buffer, 0, sizeof buffer);

                    ssize_t bytes_read = read(events[i].data.fd, buffer, sizeof buffer);

                    if (bytes_read > 0){
                        std::cout << "接收来自客户端(fd = " << events[i].data.fd << ")的消息: " << buffer << std::endl;
                        handle_error(write(events[i].data.fd, buffer, sizeof buffer) == -1, "write失败");
                    }else if (bytes_read == -1 && errno == EINTR){  // 客户端正常中断，继续读取
                        std::cout << "继续读取" << std::endl;
                        continue;
                    }else if (bytes_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)){   // 数据全部被读取
                        std::cout << "完成一次读取, errno = " << errno << std::endl;
                        break;
                    }else if(bytes_read == 0){  // 客户端断开连接
                        std::cout << "EOF, 客户端(fd = " << events[i].data.fd << ")断开连接" << std::endl;
                        handle_error(close(events[i].data.fd) == -1, "关闭epoll上的套接字失败");
                        break;
                    }
                }
            }
        }
    }

    handle_error(close(sock_fd) == -1, "关闭sock_fd失败");

    return 0;
}