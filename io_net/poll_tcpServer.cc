/*
 *  用poll 实现的高并发服务器
 *  
 *
 *
 */

#include <stdio.h>
#include <error.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/poll.h>


bool AddPoll(struct pollfd* fd_list, int listsize, int sockfd)
{
    for (int i = 0; i < listsize; i++) {
        if (fd_list[i].fd == -1) {
            fd_list[i].fd = sockfd;
            fd_list[i].events = POLLIN;
            return true;
        }
    }
    return false;
}


int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("error for input!\n");
        exit(0);
    }

    //获取地址信息等
    struct sockaddr_in addr; 
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    socklen_t len = sizeof(struct sockaddr);

    //创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("error for socket!\n");
        exit(0);
    }

    //绑定套接字
    int bindret = bind(sockfd, (struct sockaddr*)&addr, len);
    if (bindret < 0) {
        perror("error for bind\n");
        exit(0);
    }

    //监听
    int backlong = 5;
    int listenret = listen(sockfd, backlong);
    if (listenret < 0) {
        perror("error for listen!\n");
        exit(0);
    }

    //创建poll监听   
    //int poll(struct pollfd *fds, nfds_t nfds, int timeout);   
    //struct pollfd {
    //  int   fd;         /* file descriptor */
    //  short events;     /* requested events */
    //  short revents;    /* returned events */
    //};
    struct pollfd fd_list[1024]; 
    for (int i = 0; i < 1024; i++) {
        fd_list[i].fd = -1;     //默认文件描述符为-1
        fd_list[i].events = 0;  //
        fd_list[i].revents = 0;
    }
    //添加第一个listen的fd
    fd_list[0].fd = sockfd;
    fd_list[0].events = POLLIN;
    for(;;) {
        int pollret = poll(fd_list, sizeof(fd_list)/sizeof(fd_list[0]), 3000);
        if (pollret < 0) {
            perror("error for poll!\n");
            exit(0);
        }
        if (pollret == 0) {
            printf("timeout!\n");
            continue;
        }
        for (int i = 0; i < 1024; i++) {
            if (fd_list[i].fd == -1) {
                continue;
            }
            if (!(fd_list[i].revents & POLLIN)) {
                continue;
            }
            if (fd_list[i].fd == sockfd) {
                //有新的连接加入
                struct sockaddr_in cliaddr; //客户端的连接来了
                int clifd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);        
                if (clifd < 0) {
                    perror("error for accept!\n");
                    continue;   //这里失败 说明 可能是 listen 的队列满了
                }
                AddPoll(fd_list, sizeof(fd_list)/sizeof(fd_list[0]), clifd);
            }else {
                char Buffer[1024] = {0};
                int rdSize = read(fd_list[i].fd,  Buffer, 1023);
                if (rdSize < 0) {
                    perror("error for read!\n");
                    continue;
                }else if (rdSize == 0) {
                    printf("peer shutdown!\n");
                    close(fd_list[i].fd);        
                    fd_list[i].fd = -1;
                }else {
                    printf("ClientSay: %s\n", Buffer);
                    write(fd_list[i].fd, "hello world!\n", strlen("hello world!\n"));
                }
            }
        }
    }
    close(sockfd);
    return 0;
}
