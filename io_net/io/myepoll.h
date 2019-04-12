/*  使用c++封装epoll接口
 */
#include <iostream>
#include <vector>
#include <sys/epoll.h>
#include <fcntl.h>
#include "tcp_com.h"

void setnonblock(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}
#define EPOLL_MAX 10
class Epoll
{
    private:
        int _epfd;
        struct epoll_event evs[EPOLL_MAX];
    public:
        Epoll() {
            _epfd = epoll_create(EPOLL_MAX);
            if (_epfd < 0) {
                perror("epoll_create error");
                exit(-1);
            }
        }
        bool Add(TcpSocket &sock) {
            int fd = sock.GetFd();
            //int  epoll_ctl(int  epfd, int op, int fd, 
            //  struct epoll_event *event);
            //  epfd:   epoll句柄
            //  op：    操作
            //      EPOLL_CTL_ADD   添加监控事件
            //      EPOLL_CTL_MOD   监控事件修改
            //      EPOLL_CTL_DEL   移除监控事件
            //  fd：    监控的描述符
            //  event： 描述符所关注的事件
            //      EPOLLIN 可读事件
            //      EPOLLET 边缘触发模式(只有新数据到来，才会触发事件)
            //      EPOLLLT epoll的默认触发方式（水平触发）（接收缓冲区
            //      数据大小大于低水位标记，则触发事件）
            setnonblock(fd);
            struct epoll_event ev;
            ev.data.fd = fd;
            ev.events = EPOLLIN | EPOLLET;
            epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
            return true;
        }
        bool Del(TcpSocket &sock) {
            int fd = sock.GetFd();
            epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
            return true;
        }
        bool Wait(std::vector<TcpSocket> *list) {
            //int epoll_wait(int epfd, struct epoll_event *events,
            //int maxevents, int timeout);
            //  epfd:   epoll句柄
            //  events：    事件数组（用于接收就绪的描述符事件）
            //  maxevents： 事件数组的大小
            //  timeout：   等待超时时间（毫秒）
            //  返回值：<0 出错     ==0 超时   >0 就绪的描述符个数
            struct epoll_event evs[EPOLL_MAX];
            int nfds = epoll_wait(_epfd, evs, EPOLL_MAX, 3000);
            if (nfds < 0) {
                perror("epoll_wait error");
                return false;
            }else if (nfds == 0) {
                printf("time out\n");
                return true;
            }
            for (int i = 0; i < nfds; i++) {
                if (evs[i].events & EPOLLIN) {
                    TcpSocket sock(evs[i].data.fd);
                    list->push_back(sock);
                }
            }
            return true;
        }
};
