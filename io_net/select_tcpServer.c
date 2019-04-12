/*
 *  这是一个用C实现的tcp服务端
 *  用select 模型 实现多个客户端访问服务器
 *
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>


int main(int argc, char* argv[])
{

    if (argc != 3) {
        printf("error for input!\n");
        exit(0);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    socklen_t len = sizeof(struct sockaddr);

    //创建套接字
    int socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (socketfd == -1) {
        perror("error for creatsocket!\n");
        exit(0);
    }

    //绑定套接字
    int bindret = bind(socketfd, (struct sockaddr*)&addr, len);  
    if (bindret == -1) {
        perror("error for bind!\n");
        exit(0);
    }

    //监听
    int backlong = 5;
    int listenret = listen(socketfd, backlong);
    if (listenret == -1) {
        perror("error for listen!\n"); 
        exit(0);
    }
    fd_set clientset, tempset;
    int maxfd = 0;
    struct timeval waittime;
    FD_ZERO(&clientset);
    FD_SET(socketfd, &clientset);
    maxfd = socketfd + 1;
    while(1) {
        //设置超时时间
        waittime.tv_sec = 3;
        memcpy(&tempset, &clientset, sizeof(fd_set));
        int selectret = select(maxfd, &tempset, NULL, NULL, &waittime);
        if (selectret < 0) {
            perror("select error!\n");
            continue;
        }else if (selectret == 0) {
            printf("time out\n");
        }
        else {
            if (FD_ISSET(socketfd, &tempset)) {
                //新的客户端请求
                int newsocket = accept(socketfd, (struct sockaddr*)&addr, &len);
                if (newsocket < 0) {
                    perror("error for accept!\n");
                    continue;
                }
                maxfd = ((--maxfd) > newsocket ? maxfd : newsocket) + 1;
                FD_SET(newsocket, &clientset);
            }else {
                for (int i = 0; i < maxfd; i++) {
                    if (FD_ISSET(i, &tempset)) {
                        char buf[1024] = {0}; 
                        //接收
                        int rdSize = recv(i, buf, 1023, 0);
                        if (rdSize == 0) {
                            //返回为0 说明对端关闭
                            printf("peer shutdown!\n");
                            close(i);
                            FD_CLR(i, &clientset);
                            for (size_t j = maxfd; j > 0; j--) {
                                if (!FD_ISSET(j, &clientset)) {
                                    continue;
                                }
                                maxfd = j + 1;
                            }
                        }else if (rdSize < 0) {
                            //发生错误
                            printf("error for receiver!\n");
                            continue;
                        }else {
                            printf("Client  Say :%s\n", buf);
                            //发送
                            //printf("Server  Say :");     
                            //fflush(stdout);
                            //memset(buf, 0, 1024);
                            //scanf("%s", buf);
                            //int sendSize = send(i, buf, strlen(buf), 0);
                            int sendSize = send(i, "hello", strlen("hello"), 0);
                            if (sendSize < 0) {
                                perror("error for send!\n");
                                continue;
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

