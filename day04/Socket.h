// Copyright [2023] <Wanfeng Huang>

#ifndef DAY04_SOCKET_H_
#define DAY04_SOCKET_H_

#include <arpa/inet.h>

class Socket{
 private:
    int fd;

 public:
    Socket(int domain, int type, int protocol);
    ~Socket();

    int get_fd();
    void sock_bind(in_addr_t ip, sa_family_t family, in_port_t port);
    void sock_listen(int n);
};

#endif  // DAY04_SOCKET_H_
