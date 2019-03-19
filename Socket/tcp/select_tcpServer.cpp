/*
 *  这是一个用C++实现的tcp服务端
 *  用select 模型 实现多个客户端访问服务器
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>


int main(int argc, char* argv[])
{

    if (argc != 3) {
        printf("error for input");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    socklen_t len = sizeof(sockaddr);

    //创建套接字
    int socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (socketfd == -1) {
        perror("error for creatsocket\n");
        exit(0);
    }

    //绑定套接字
    int bindret = bind(socketfd, (struct sockaddr*)&addr, len);  
    if (bindret == -1) {
        perror("error for bind\n");
        exit(0);
    }

    //监听
    int backlong = 5;
    int listenret = listen(socketfd, backlong);
    if (listenret == -1) {
        perror("error for listen\n"); 
        exit(0);
    }
    fd_set clientset;
    FD_ZERO(&clientset);
    int fd_list[1024];
    int maxfd = 0;
    memset(fd_list, -1, sizeof(int) * 1024);
    fd_list[0] = socketfd;
    maxfd++;
    struct timeval waittime;
    while(1) {
        waittime.tv_sec = 3;
        for (size_t i = 0; i < maxfd; i++) {
            if (fd_list[i] != -1) {
                FD_SET(fd_list[i], &clientset);
            }
        }
        int selectret = select(maxfd, &clientset, NULL, NULL, &waittime);
        if (selectret < 0) {
            perror("select error\n");
            exit(0);
        }else if (selectret == 0) {
            printf("time out\n");
        }
        else {
            if (FD_ISSET(socketfd, &clientset)) {
                //新的客户端请求
                sockaddr_in cliaddr;
                socklen_t clilen = sizeof(sockaddr_in);
                int newsocket = accept(socketfd, (struct sockaddr*)&addr, &len);
                if (newsocket < 0) {
                    perror("error for accept\n");
                    exit(0);
                }
                fd_list[maxfd++] = newsocket;
            }else {
                for (size_t i = 0; i < maxfd; i++) {
                    if (FD_ISSET(fd_list[i], &clientset)) {
                        char buf[1024] = {0}; 
                        //接收
                        int rdSize = recv(fd_list[i], buf, 1023, 0);
                        if (rdSize = 0) {
                            //返回为0 说明对端关闭
                            printf("peer shutdown!\n");
                            close(fd_list[i]);
                            fd_list[i] = -1;
                            //调整maxfd
                            for (size_t i = maxfd - 1; i > 0; i--) {
                                if (fd_list[i] != -1) {
                                    maxfd = i + 1; 
                                    break;
                                } 
                            }
                        }else if (rdSize < 0) {
                            //发生错误
                            printf("error for receiverSocket");
                            exit(0);
                        }else {
                            printf("Client  Say :%s\n", buf);
                            //发送
                            printf("Server  Say :");     
                            fflush(stdout);
                            memset(buf, 0, 1024);
                            scanf("%s", buf);
                            size_t sendSize = send(fd_list[i], buf, strlen(buf), 0);
                            if (sendSize < 0) {
                                perror("error for sendSocket\n");
                                exit(0);
                            }
                        }
                    } 
                }
            }
        }
    }
    close(socketfd);

    return 0;
}

