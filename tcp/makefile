.PHONY:test
test:select_tcpServer tcpClient
select_tcpServer:select_tcpServer.cpp
	g++ $^ -o $@ -lpthread
tcpClient:tcpClient.cpp
	g++ $^ -o $@
