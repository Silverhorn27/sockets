#include <iostream>
#include <algorithm>
#include <set>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    int master_socket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);
    
    std::set<int> slave_sockets;
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(12343);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(master_socket, (struct sockaddr *)(&sock_addr), sizeof(sock_addr));
    listen(master_socket, SOMAXCONN);

    while(true) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(master_socket, &set);
        for(auto &socket : slave_sockets) {
            FD_SET(socket, &set);
        }

        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        int max = std::max(master_socket, *std::max_element(slave_sockets.begin(), slave_sockets.end()));
        select(max+1, &set, NULL, NULL, NULL);
        for(auto &socket : slave_sockets) {
            if (FD_ISSET(socket, &set)) {
                if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                    std::cout << ("setsockopt faile\n");
                }
                if (setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                    std::cout << ("setsockopt faile\n");
                }
                static char buffer[1024];
                int recv_size = recv(socket, buffer, 1024, MSG_NOSIGNAL);
                if (recv_size <= 0) {
                    if (recv_size < 0) {
			std::cout << "error. recv_size is: " << recv_size << std::endl;
		    }
                    close(socket);
                    slave_sockets.erase(socket);
                } else {
                    send(socket, buffer, recv_size, MSG_NOSIGNAL);
                }
            }
        }
        if (FD_ISSET(master_socket, &set)) {
            int slave_socket = accept(master_socket, 0, 0);
            slave_sockets.insert(slave_socket);
        }
    }

    return 0;
}
