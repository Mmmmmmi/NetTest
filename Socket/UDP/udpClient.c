#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <error.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
int main()
{
    //1. 创建套接字
    int socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketfd == -1) {
        perror("error for socket");
        return -1;
    }


    //2. 绑定套接字 //客户端不推荐绑定
    /*struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("10.0.6.118");
    socklen_t len = sizeof(client_addr);
    int ret = bind(socketfd, (struct sockaddr*)&client_addr, len);
    if (ret == -1) {
        perror("error for bind");
        return -1;
    }
    */
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7777);
    //service_addr.sin_addr.s_addr = inet_addr("10.0.5.226");
    server_addr.sin_addr.s_addr = inet_addr("119.23.248.167");
    socklen_t len = sizeof(struct sockaddr_in);
    while(1) {
        //3. 发送信息
        char buf[1024] = {0};
        printf("You want to say:");
        scanf("%s", buf);
        printf("send start\n");
        int sendret = sendto(socketfd, buf, strlen(buf), 0, (struct sockaddr*)&server_addr, len);
        if (sendret == -1) {
            perror("error for sendto");
            return -1;
        }
        printf("send end\n");
        //4. 接收信息
        memset(buf, 0, 1024);
        printf("recvfrom start\n");
        recvfrom(socketfd, buf, 1023, 0, (struct sockaddr*)&server_addr, &len);
        printf("Server say: %s\n", buf);
        printf("recvfrom start\n");
    }

    //5. 关闭套接字
    close(socketfd);
    return 0;
}
