all:epoll_tcpServer tcpClient
epoll_tcpServer:epoll_tcpServer.cc
	gcc $^ -o $@ -lpthread 
select_tcpServer:select_tcpServer.c
	gcc $^ -o $@ -lpthread
tcpClient:tcpClient.c
	gcc $^ -o $@
.PHONY:clean
clean:
	rm -rf select_tcpServer tcpClient
