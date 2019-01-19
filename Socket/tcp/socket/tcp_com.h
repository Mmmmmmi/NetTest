/*  编写一个tcp socket类，能够实现socket的接口封装使用
 *      创建套接字
 *      绑定地址信息
 *      监听
 *      获取新连接
 *      发起连接请求
 *      发送数据
 *      接收数据
 *      关闭套接字
 */
#ifndef __M_TCPCOM_H__
#define __M_TCPCOM_H__
#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CHECK_RET(q) if ((q) == false) {return -1;}
class TcpSocket
{
    private:
        int _sockfd;
    public:
        TcpSocket():_sockfd(-1) {}
        TcpSocket(int fd) {  _sockfd = fd; }
        int GetFd() {
            return _sockfd;
        }
        //创建套接字
        bool Socket()
        {
            _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (_sockfd < 0) {
                perror("socket error");
                return false;
            }
            return true;
        }
        //绑定地址信息
        bool Bind(std::string &ip, uint16_t port)
        {
            sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(sockaddr_in);
            int ret = bind(_sockfd, (sockaddr*)&addr, len);
            if (ret < 0) {
                perror("bind error");
                return false;
            }
            return true;
        }
        //开始监听（服务端）
        bool Listen(int backlog = 5)
        {
            //int listen(int sockfd, int backlog);
            //开始监听，并且确定最大并发连接数
            int ret = listen(_sockfd, backlog);
            if (ret < 0) {
                perror("listen error");
                return false;
            }
            return true;
        }
        //获取服务端为新客户端所建立的socket连接（服务端）
        bool Accept(TcpSocket *sock, sockaddr_in *cliaddr = NULL)
        {
            //int accept(int sockfd, struct sockaddr *addr, 
            //socklen_t *addrlen);
            //从已完成连接队列中获取已经客户端建立连接的socket
            //sockfd:   套接字描述符
            //addr：    客户端地址信息
            //addrlen： 地址信息长度
            //返回值：客户端新连接的socket描述符， 失败：-1
            socklen_t len = sizeof(sockaddr_in);
            sockaddr_in addr;
            int newcli = accept(_sockfd, (sockaddr*)&addr, &len);
            if (newcli < 0) {
                perror("accpet error");
                return false;
            }
            sock->_sockfd = newcli;
            if (cliaddr != NULL) memcpy(cliaddr, &addr, len);
            return true;
        }
        //发起连接请求 (客户端)
        bool Connect(std::string &ip, uint16_t port)
        {
            //int connect(int sockfd, const struct sockaddr *addr,
            //  socklen_t addrlen);
            //向服务端发起连接请求
            sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(sockaddr_in);
            int ret = connect(_sockfd, (sockaddr*)&addr, len);
            if (ret < 0) {
                return false;
            }
            return true;
        }
        //发送数据
        ssize_t Send(const char *buf, size_t len)
        {
            //ssize_t send(int sockfd, const void *buf, size_t len, 
            //int flags);
            //因为tcp中服务端会为客户端新建socket进行专项通信，因此发送
            //或接收数据就不需要用户来指定对端的地址信息
            ssize_t ret = send(_sockfd, buf, len, 0);
            if (ret < 0) {
                perror("send error");
            }
            return ret;
        }
        //接收数据
        ssize_t Recv(char *buf, size_t len)
        {
            //ssize_t recv(int sockfd, void *buf, size_t len,int flags);
            //接收数据
            ssize_t ret = recv(_sockfd, buf, len, 0);
            if (ret < 0) {
                perror("recv error");
            }else if (ret == 0) {
                printf("peer shutdown!\n");
            }
            return ret;
        }
        //关闭套接字
        bool Close()
        {
            if (_sockfd != -1) {
                close(_sockfd);
                _sockfd = -1;
            }
            return true;
        }
};
#endif
