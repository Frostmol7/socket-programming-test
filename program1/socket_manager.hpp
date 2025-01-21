#ifndef SOCKET_MANAGER_HPP
#define SOCKET_MANAGER_HPP

#include <string>

class SocketManager {
public:
    explicit SocketManager(int port);
    ~SocketManager();

    bool initialize();
    bool send_data(const std::string &data);

private:
    int port;
    int server_fd;
    int client_fd;
};

#endif // SOCKET_MANAGER_HPP

