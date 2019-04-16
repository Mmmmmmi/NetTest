#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("input error!\n");
        exit(0);
    }

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
        perror("error for bind!\n");
        exit(0);
    }
   
    //监听
    int backlong = 5;
    int listenret = listen(sockfd, backlong);
    if (listenret < 0) {
        perror("error for listen!\n");
        exit(0);
    }

    //创建epoll 文件描述符
    int epollfd = epoll_create(10);
    if (epollfd < 0) {
        perror("error for epoll_create!\n");
        exit(0);
    }

    //将sockfd 添加到 epoll 监听
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sockfd;
    //int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
    int epoll_ctl_ret = epoll_ctl(epollfd, EPOLL_CTL_ADD,  sockfd, &event);
    if (epoll_ctl_ret < 0) {
        perror("error for epoll_ctl!\n");
        exit(0);
    } 
    for (;;) {
        struct epoll_event events[10];
        //int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
        int size = epoll_wait(epollfd, events, sizeof(events)/ sizeof(events[0]), 3000);
        if (size < 0) {
            perror("error for epoll_wait!\n");
            continue;
        }
        if (size == 0) {
            printf("timeout!\n");
            continue;
        }
        for (int i = 0; i < size; i++) {
            if (!(events[i].events & EPOLLIN)) {
                continue;
            }
            if (events[i].data.fd == sockfd) {
                printf("new Client !\n");
                //添加新链接
                struct sockaddr_in cliaddr;
                socklen_t clilen;
                int acceptret = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
                if (acceptret < 0) {
                    perror("error for accept!\n");
                    continue;
                }
                //新的文件描述符添加到epoll中
                struct epoll_event event;
                event.data.fd = acceptret;
                event.events = EPOLLIN;
                int ctlret = epoll_ctl(epollfd, EPOLL_CTL_ADD, acceptret, &event);
                if (ctlret < 0) {
                    perror("error for epoll_ctl!\n");
                    continue;
                }
                printf("add new Client finish !\n");
            }else {
                printf("recv and send!\n");
                //接收数据
                char buffer[1024] = {0};
                int rdSize = read(events[i].data.fd, buffer, sizeof(buffer) - 1);
                if (rdSize < 0) {
                    perror("error for read!\n");
                    continue;
                }
                if (rdSize == 0) {
                    printf("peer shutdown!\n");
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                    continue;
                }
                printf("Client Say: %s\n", buffer);
                write(events[i].data.fd, "hello world!\n", sizeof("hello world!\n"));
            }
        }
    }


    return 0;
}

