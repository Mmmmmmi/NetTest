/*
 *  这是一个用C++实现的tcp服务端
 *  用多线程实现一个服务器对应多个服务端
 *
 */





#include "tcpHead.h"

void *thread_do(void *arg)
{
    TcpSocket *tcpsocket = (TcpSocket*)arg;
    while(1) {

        char buf[1024] = {0}; 

        //接收
        tcpsocket->receiveSocket(buf, 1023);
        printf("Client  Say :  %s\n", buf);


        //发送
        printf("Server  Say : ");     
        fflush(stdout);
        memset(buf, 0, 1024);
        scanf("%s", buf);
        tcpsocket->sendSocket(buf, strlen(buf));
    }
    tcpsocket->closeSocket();
    return NULL;
}



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


    //进行数据沟通
    while(1) {

        TcpSocket *newsocket = new TcpSocket;

        //如果没有客户端就阻塞，如果完成队列满了就返沪错误
        if (tcpserver.acceptSocket(*newsocket) == false) {
            continue;
        }

        pthread_t tid; 
        pthread_create(&tid, NULL, thread_do, (void*)newsocket);
        pthread_detach(tid);
    }
    tcpserver.closeSocket();
    return 0;
}

