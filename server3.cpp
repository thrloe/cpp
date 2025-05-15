#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    int server_fd, client_sock;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buf[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "waiting" << std::endl;
    client_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    std::cout << "client connected" << std::endl;

    read(client_sock, buf, 1024);
    std::string name(buf);
    std::string greeting = "hello " + name;

    send(client_sock, greeting.c_str(), greeting.length(), 0);

    memset(buf, 0, sizeof(buf));
    while (true) {
        int valread = read(client_sock, buf, 1024);
        if (valread <= 0) break;

        buf[valread] = '\0';
        std::cout << "message:  " << buf << std::endl;
        memset(buf, 0, sizeof(buf));
    }

    close(client_sock);
    close(server_fd);
    return 0;
}
