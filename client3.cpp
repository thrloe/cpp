#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    std::string name;
    std::cout << "name: ";
    std::getline(std::cin, name);
    send(sock, name.c_str(), name.length(), 0);

    read(sock, buffer, 1024);
    std::cout << "server`s answer: " << buffer << std::endl;

    while (true) {
        std::string msg;
        std::cout << "msg? (or 'exit' for exit): ";
        std::getline(std::cin, msg);
        if (msg == "exit") break;
        send(sock, msg.c_str(), msg.length(), 0);
    }

    close(sock);
    return 0;
}
