all:select_tcpServer tcpClient
select_tcpServer:select_tcpServer.c
	gcc $^ -o $@ -lpthread
tcpClient:tcpClient.c
	gcc $^ -o $@
.PHONY:clean
clean:
	rm -rf select_tcpServer tcpClient
