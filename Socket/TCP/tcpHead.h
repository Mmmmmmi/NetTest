#ifndef __TCP_HEAD_H__
#define __TCP_HEAD_H__

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>


#define CHECK_RET(i) if((i) == false) {return -1;} 

class TcpSocket
{
public:

    //构造函数
    TcpSocket()
        :_socketfd(-1)
    {}

    bool creatSocket()  //创建套接字
    {
        //创建套接字
        _socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_socketfd == -1) {
            perror("error for creatSocket\n");
            return false;
        }
        return true;
    }

    bool bindSocket(std::string &ip, uint16_t port)
    {
        //传入ip地址 和 端口号 port
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(port);
        socklen_t len = sizeof(sockaddr);
        int ret = bind(_socketfd, (struct sockaddr*)&addr, len);
        if (ret == -1) {
            perror("error for bindSocket");
            return false;
        }
        return true;
    }
    
    bool listenSocket(int backlog = 5)
    {
        //int listen(int sockfd, int backlog); 
        // backlog 同一时间TCP的最大并发连接数
        int ret = listen(_socketfd, backlog);
        if (ret == -1) {
            perror("error for listenSocket");
            return false;
        }
        return true;
    }

    bool acceptSocket(TcpSocket& newsocket, sockaddr_in * clientaddr = NULL)
    {
        //newsocket 是返回新创建的socket
        //clientaddr 是如果服务器需要看一下客户端的地址，可以传入参数等待返回后查看
        //如果不需要 就直接是null
        //int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
        socklen_t len = sizeof(sockaddr_in);
        sockaddr_in addr;
        int ret = accept(_socketfd, (struct sockaddr*)&addr, &len);
        if (ret < 0) {
            perror("error for acceptSocket"); 
            return false;
        }
        newsocket._socketfd = ret;
        if (clientaddr != NULL) {
            memcpy(clientaddr, &addr, len);
        }
        return true;
    }

    bool connectSocket(std::string& ip, uint16_t port)
    {   
        //传入服务器端的地址信息和端口号
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(port);
        socklen_t len = sizeof(sockaddr_in);
        //int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
        int ret = connect(_socketfd, (struct sockaddr*)&addr, len);
        if (ret == -1) {
            return false;
        }
        return true;
    }

    ssize_t sendSocket(const char *buf, size_t len)
    {
        //发送数据
        //ssize_t send(int sockfd, const void *buf, size_t len, int flags);
        //因为tcp中服务端会为客户端新建socket进行专项通信，因此发送
        //或接收数据就不需要用户来指定对端的地址信息
        //返回值 ：成功　　实际发送的字节数 失败　－１
        ssize_t ret = send(_socketfd, buf, len, 0);
        if (ret < 0) {
            perror("error for sendSocket");
        }
        return ret;
    }

    ssize_t receiveSocket(char *buf, size_t len)
    {
        //接收数据  返回实际接收的数据数
        //ssize_t recv(int sockfd, void *buf, size_t len, int flags);
        //返回值 ：成功　　实际发送的字节数 失败　－１
        ssize_t ret = recv(_socketfd, buf, len, 0);
        if (ret < 0) {
            perror("error for receiveSocket");
        }else if (ret == 0) {
            printf("peer shutdown!\n");
        }
        return ret;
    }

    bool closeSocket()
    {
        if (_socketfd != -1) {
            close(_socketfd);
            _socketfd = -1;
        }
        return true;
    }

    ~TcpSocket()
    {}
private:
    int _socketfd;      //tcp的文件描述符
};
#endif
