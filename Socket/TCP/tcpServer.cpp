#include "tcpHead.h"

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
        
        char buf[1024] = {0}; 

        //接收
        newsocket.receiveSocket(buf, 1023);
        printf("Client  Say :  %s\n", buf);


        //发送
        printf("Server Say : ");     
        fflush(stdout);
        memset(buf, 0, 1024);
        scanf("%s", buf);
        newsocket.sendSocket(buf, strlen(buf));
        
    }
    tcpserver.closeSocket();
    
    return 0;
}

