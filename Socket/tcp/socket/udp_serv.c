/*  一个基于udp协议的网络通信服务端程序
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main()
{
    //1. 创建套接字
    //  int socket(int domain, int type, int protocol);
    //  domain: 地址域      AF_INET 使用IPV4网络协议
    //  type：  套接字类型
    //          SOCK_STREAM 流式套接字      默认协议tcp
    //          SOCK_DGRAM  数据报套接字    默认协议udp
    //  protocol：协议
    //      IPPROTO_UDP 6     UDP协议 
    //      IPPROTO_TCP 17    TCP协议
    //  返回值：套接字描述符（非负整数）    失败：-1
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket error");
        return -1;
    }
    //2. 为套接字绑定地址信息
    //int bind(int sockfd, struct sockaddr *addr,socklen_t addrlen);
    //  sockfd: 套接字描述符
    //  addr：  地址信息
    //      使用sockaddr_in定义，使用时进行类型强转
    //  addrlen：   地址信息长度
    //  返回值：0       失败：-1
    struct sockaddr_in addr;
    addr.sin_family  = AF_INET;
    //uint16_t htons(uint16_t hostshort);
    //将uint16_t类型的数据从主机字节序转换为网络字节序
    addr.sin_port = htons(9000);
    //uint32_t htonl(uint32_t hostlong);
    //将uint32_t类型的数据从主机字节序转换为网络字节序
    //in_addr_t inet_addr(const char *cp);
    //将点分十进制字符串ip地址转换为网络字节序ip地址
    addr.sin_addr.s_addr = inet_addr("192.168.122.135");
    //addr.sin_addr.s_addr = htonl(0xc0a87a87);
    socklen_t len = sizeof(struct sockaddr_in);
    int ret;
    ret = bind(sockfd, (struct sockaddr*)&addr, len);
    if (ret < 0) {
        perror("bind error\n");
        return -1;
    }
    while(1) {
        //3. 接收数据
        //  ssize_t recvfrom(int sockfd, void *buf, size_t len, 
        //  int flags,struct sockaddr *src_addr, socklen_t *addrlen);
        //  sockfd: 套接字描述符---确定从哪里获取数据
        //  buf：   用于接收数据
        //  len：   接收的数据长度
        //  flags： 默认赋0---阻塞接收（没数据就一直等待）
        //  src_addr：  数据从哪里来，对端地址信息
        //  addrlen：   地址信息长度（输入输出复合参数）
        //  返回值：实际接收的数据长度      失败：-1
        char buff[1024] = {0};
        struct sockaddr_in cli_addr;
        recvfrom(sockfd, buff, 1023, 0, 
                (struct sockaddr*)&cli_addr, &len);
        printf("client say: %s\n", buff);
        //4. 发送数据
        memset(buff, 0x00, 1024);
        scanf("%s", buff);
        //ssize_t sendto(int sockfd, const void *buf, size_t len, 
        //  int flags,struct sockaddr *dest_addr, socklen_t addrlen);
        //  dest_addr:  目的端地址信息
        //  addrlen：   地址信息长度
        sendto(sockfd, buff, strlen(buff), 0, 
                (struct sockaddr*)&cli_addr,len);
    }
    //5. 关闭套接字
    close(sockfd);
    return 0;
}
