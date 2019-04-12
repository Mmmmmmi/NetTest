/*  使用c++对select进行封装使用
 */

#include <iostream>
#include <vector>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include "tcp_com.h"
class Select
{
    private:
        fd_set rfds;
        int max_fd;
        struct timeval tv;
    public:
        Select() {
            FD_ZERO(&rfds);
            max_fd = -1;
        }
        bool Add(TcpSocket &sock){
            //添加描述符到select对象的监控集合中
            int fd = sock.GetFd();
            FD_SET(fd, &rfds);
            max_fd = max_fd > fd ? max_fd : fd;
            return true;
        }
        bool Del(TcpSocket &sock) {
            //从select对象的监控集合中移除描述符
            int fd = sock.GetFd();
            FD_CLR(fd, &rfds);
            for (int i = max_fd; i > 0; i--) {
                if (FD_ISSET(i, &rfds)) {
                    max_fd = i;
                    break;
                }
            }
            return true;
        }
        bool Wait(std::vector<TcpSocket> *list) {
            //对监控集合中描述符进行状态监控
            //返回就绪的描述符
            tv.tv_sec = 3;
            tv.tv_usec = 0;
            fd_set set = rfds;
            int ret = select(max_fd + 1, &set, NULL, NULL, &tv);
            if (ret < 0) {
                perror("select error");
                return false;
            }else if (ret == 0) {
                std::cout<<"time out\n";
                return true;
            }
            for (int i = 0; i < max_fd + 1; i++) {
                if (FD_ISSET(i, &set)) {
                    TcpSocket sock(i);
                    list->push_back(sock);
                }
            }
            return true;
        }
};
