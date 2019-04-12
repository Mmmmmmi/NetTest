/*
 *  这是一个用C实现的tcp客户端
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("error for input\n");
        exit(0);
    }

    //确定服务器地址和端口
    struct sockaddr_in serverAddr;  //服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_port = htons(atoi(argv[2]));
    socklen_t len = sizeof(struct sockaddr);

    //创建套接字
    int socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //创建套接字       域(ip地址类型) 传输数据类型   协议
    if (socketfd < 0) {
        perror("error for creatsocket\n");
        exit(0);
    }
    //到这里 套接字创建成功

    //绑定套接字 //这里不推荐绑定  让操作系统自己绑定
    //到这里 套接字绑定成功

    //连接
    connect(socketfd, (struct sockaddr*)&serverAddr, len);

    while(1) {
        //收发数据
        char buffer[1024];
        memset(buffer, 0, 1024);
        printf("Client Say :");
        fflush(stdout);
        scanf("%s", buffer);
        int sendret = send(socketfd, buffer, strlen(buffer), 0);
        if (sendret < 0) {
            perror("error for send");
            exit(0);
        }
        memset(buffer, 0, 1024);
        int recvret = recv(socketfd, buffer, 1023, 0);
        if (recvret < 0) {
            perror("error for send");
            exit(0);
        }
        printf("Server Say :%s\n", buffer);
    }
    close(socketfd);
    return 0;
}
