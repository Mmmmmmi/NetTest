ALL:tcp_cli ttcp_srv
.PHONY:tcp_cli tcp_srv
ttcp_srv:ttcp_srv.cpp
	g++ $^ -o $@ -pthread
ptcp_srv:ptcp_srv.cpp
	g++ $^ -o $@
tcp_cli:tcp_cli.cpp
	g++ $^ -o $@
tcp_srv:tcp_srv.cpp
	g++ $^ -o $@
udp_cli:udp_cli.cpp
	g++ $^ -o $@
udp_serv:udp_serv.c
	gcc $^ -o $@
clean:
rebuild: clean udp_cli
