#include "tcpHead.h"

int main(int argc, char *argv[])
{


    TcpSocket tcpclient;

    if (argc != 3) {
        printf("error for input\n");
        return -1;
    }

    std::string serviceip = argv[1];
    uint16_t serviceport = atoi(argv[2]);


    //创建套接字
    CHECK_RET(tcpclient.creatSocket());

    //绑定套接字
    //客户端不推荐绑定

    //连接
    CHECK_RET(tcpclient.connectSocket(serviceip, serviceport));

    while(1) {
        //发送数据
        char buf[1024] = {0};
        printf("Client  Say : ");
        fflush(stdout);
        scanf("%s", buf);
        tcpclient.sendSocket(buf, strlen(buf));

        //接收数据
        memset(buf, 0, 1024);

        tcpclient.receiveSocket(buf, 1023);
        printf("Service Say : %s\n", buf);
    }
    tcpclient.closeSocket();
    return 0;
}

