/*
 *  这是一个用C++实现的tcp服务端，使用了多进程来保证实现一个服务器对应多个客户端
 *
 */

#include "tcpHead.h"
#include <sys/wait.h>
#include <cstdlib>

int main(int argc, char* argv[])
{

    TcpSocket tcpserver;
    
    if (argc != 3) {
        printf("error for input");
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    //创建套接字
    CHECK_RET(tcpserver.creatSocket());

    //绑定套接字
    CHECK_RET(tcpserver.bindSocket(ip, port));


    //监听
    CHECK_RET(tcpserver.listenSocket(5));

    while(1) {

        TcpSocket newsocket;
        //获取新的文件描述符 
        if (tcpserver.acceptSocket(newsocket) == false) {
            continue;
        }

        int forkret = fork();
        if (forkret == -1) {
            perror("error for fork");
            return -1;
        }else if (forkret == 0) {
            //第一层子进程          由于进程无法分离，所以直接创建两个子进程，让第一个子进程直接返回，这样第二个子进程就成了孤儿进程，系统就会回收它
            int retfork = fork();
            if (retfork == -1) {
                perror("error for fork");
                return -1;
            }else if (retfork == 0){
                //第二层子进程

                char buf[1024] = {0}; 
                while(1) {

                    //接收
                    newsocket.receiveSocket(buf, 1023);
                    printf("Client  Say :  %s\n", buf);


                    //发送
                    printf("Server  Say : ");     
                    fflush(stdout);
                    memset(buf, 0, 1024);
                    scanf("%s", buf);
                    newsocket.sendSocket(buf, strlen(buf));
                }
                newsocket.closeSocket();
                exit(0);
            }else {
                exit(0);
            }

        }else {
            newsocket.closeSocket();
            wait(NULL);
        }
    }
    tcpserver.closeSocket();
    
    return 0;
}

