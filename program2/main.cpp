#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include <cstring>

constexpr int PORT = 8080;
constexpr size_t BUFFER_SIZE = 1024;

void analyze_and_print(const std::string &data) {
    std::map<char, int> char_count;
    for (char c : data) {
        if (isalpha(c)) {
            char_count[c]++;
        }
    }

    for (const auto &pair : char_count) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }
}

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    while (true) {
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            std::cerr << "Connection lost or closed by server.\n";
            break;
        }

        buffer[bytes_received] = '\0';
        std::string received_data(buffer);
        std::cout << "Received data: " << received_data << "\n";
        analyze_and_print(received_data);
    }

    close(client_socket);
    return 0;
}

