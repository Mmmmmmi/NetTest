/*
 *  这是一个用C++实现的tcp服务端，目前只能对应一个客户端
 *
 *
 */


#include "tcpHead.h"

int main(int argc, char* argv[])
{

    TcpSocket tcpserver;
    
    if (argc != 3) {
        printf("error for input!\n");
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


    //获取新的文件描述符, 如果直接放到循环里面，并且始终只有一个客户端的话
    //在第二次循环进来时，就会陷入阻塞 
    TcpSocket newsocket;
    CHECK_RET(tcpserver.acceptSocket(newsocket));


    //进行数据沟通
    
    while(1) {


        char buf[1024] = {0}; 

        //接收
        newsocket.receiveSocket(buf, 1023);
        printf("Client  Say : %s\n", buf);


        //发送
        printf("Server  Say : ");     
        fflush(stdout);
        memset(buf, 0, 1024);
        scanf("%s", buf);
        newsocket.sendSocket(buf, strlen(buf));
        
    }
    tcpserver.closeSocket();
    
    return 0;
}
