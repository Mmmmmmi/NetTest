.PHONY:test
test:t_tcpServer tcpClient
t_tcpServer:t_tcpServer.cpp
	g++ $^ -o $@ -lpthread
p_tcpServer:p_tcpServer.cpp
	g++ $^ -o $@
tcpClient:tcpClient.cpp
	g++ $^ -o $@
