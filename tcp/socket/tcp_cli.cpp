/*  使用TcpSocket类实现tcp 的客户端程序
 *  创建套接字-》发起连接请求-》收发数据-》关闭连接
 */

#include "tcp_com.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: ./tcp_cli ip port\n");
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket sock;

    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Connect(ip, port));
    while(1) {
        std::string buf;
        std::cout << "client say:";
        fflush(stdout);
        std::cin >> buf;
        sock.Send(buf.c_str(), buf.length());
        char rbuf[1024] = {0};
        sock.Recv(rbuf, 1023);
        printf("server say: %s\n", rbuf);
    }
    sock.Close();
    return 0;
}
