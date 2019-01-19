/*  使用封装好的TcpSocket类实现tcp服务端程序
 *      创建套接字-》绑定地址-》开始监听-》获取新连接-》收发数据-》关闭
 */

#include "tcp_com.h"
#include <pthread.h>

void *thr_start(void *arg)
{
    TcpSocket *clisock = (TcpSocket*)arg;
    while(1) {
        char buff[1024] = {0};
        clisock->Recv(buff, 1023);
        printf("client say: %s\n", buff);
        memset(buff, 0x00, 1024);
        printf("server say: ");
        fflush(stdout);
        scanf("%s", buff);
        clisock->Send(buff, strlen(buff));
    }
    clisock->Close();
    delete clisock;
    return NULL;
}
int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: ./tcp_srv ip port\n");
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket sock;

    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind(ip, port));
    CHECK_RET(sock.Listen());
    while(1) {
        TcpSocket *clisock  = new TcpSocket();
        //获取新连接,阻塞函数：没有新连接则阻塞等待
        if (sock.Accept(clisock) == false) {
            continue;
        }
        pthread_t tid;
        pthread_create(&tid, NULL, thr_start, (void*)clisock);
        pthread_detach(tid);
    }
    sock.Close();
    return 0;   
}
