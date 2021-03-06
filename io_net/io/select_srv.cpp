/*  使用封装好的TcpSocket类实现tcp服务端程序
 *      创建套接字-》绑定地址-》开始监听-》获取新连接-》收发数据-》关闭
 */

#include "myselect.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: ./tcp_srv ip port\n");
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket sock;
    Select s;

    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind(ip, port));
    CHECK_RET(sock.Listen());
    s.Add(sock);
    while(1) {
        std::vector<TcpSocket> list;
        if (s.Wait(&list) == false) {
            return -1;
        }
        for (int i = 0; i < list.size(); i++) {
            if (list[i].GetFd() == sock.GetFd()) {
                TcpSocket clisock;
                if (sock.Accept(&clisock) == false) {
                    continue;
                }
                s.Add(clisock);
            }else {
                char buff[1024] = {0};
                list[i].Recv(buff, 1023);
                printf("client say: %s\n", buff);
                memset(buff, 0x00, 1024);
                printf("server say: ");
                fflush(stdout);
                list[i].Send("不想理你~", strlen("不想理你~"));
            }
        }
    }
    sock.Close();
    return 0;   
}
