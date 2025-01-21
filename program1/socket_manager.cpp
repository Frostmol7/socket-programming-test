#include "socket_manager.hpp"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

SocketManager::SocketManager(int port) : port(port), server_fd(-1), client_fd(-1) {}

SocketManager::~SocketManager() {
    if (client_fd >= 0) close(client_fd);
    if (server_fd >= 0) close(server_fd);
}

bool SocketManager::initialize() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return false;
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        return false;
    }

    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        return false;
    }

    std::cout << "Waiting for a client to connect...\n";
    client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd < 0) {
        perror("Accept failed");
        return false;
    }

    std::cout << "Client connected.\n";
    return true;
}

bool SocketManager::send_data(const std::string &data) {
    if (client_fd < 0) return false;

    ssize_t sent = send(client_fd, data.c_str(), data.size(), 0);
    return sent == static_cast<ssize_t>(data.size());
}

