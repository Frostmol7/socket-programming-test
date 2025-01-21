#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include <cstring>
#include "socket_manager.hpp"
#include <algorithm>

constexpr size_t MAX_STRING_SIZE = 64;

std::mutex buffer_mutex;
std::condition_variable buffer_cv;
bool data_ready = false;
std::string shared_buffer;

void input_thread() {
    while (true) {
        std::string input;
        std::cout << "Enter a string (letters only, max 64 characters): ";
        std::getline(std::cin, input);

        if (input.size() > MAX_STRING_SIZE || input.empty() || !std::all_of(input.begin(), input.end(), isalpha)) {
            std::cerr << "Invalid input. Try again.\n";
            continue;
        }
        
        std::map<char, int> char_count;
        for (char c : input) {
            char_count[c]++;
        }
        
        std::string buffer_data;
        for (const auto &pair : char_count) {
            buffer_data += pair.first + std::to_string(pair.second) + " ";
        }

        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            shared_buffer = buffer_data;
            data_ready = true;
        }

        buffer_cv.notify_one();
    }
}

void processing_thread(SocketManager &socket_manager) {
    while (true) {
        std::unique_lock<std::mutex> lock(buffer_mutex);
        buffer_cv.wait(lock, [] { return data_ready; });
        std::string data_to_send = shared_buffer;
        shared_buffer.clear();
        data_ready = false;

        lock.unlock();

        std::cout << "Processed data: " << data_to_send << "\n";

        if (!socket_manager.send_data(data_to_send)) {
            std::cerr << "Failed to send data.\n";
        }
    }
}

int main() {
    SocketManager socket_manager(8080);
    if (!socket_manager.initialize()) {
        std::cerr << "Failed to initialize socket.\n";
        return 1;
    }

    std::thread t1(input_thread);
    std::thread t2(processing_thread, std::ref(socket_manager));

    t1.join();
    t2.join();

    return 0;
}
