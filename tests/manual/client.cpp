#include <iostream>
#include <string>

#include "client.hpp"

namespace ip = boost::asio::ip;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        ledman::logger::error("usage: server <ip> <port>");
        return 1;
    }

    const std::string ip = argv[1];
    const unsigned short port = static_cast<unsigned short>(std::atoi(argv[2]));
    ip::tcp::endpoint endpoint(ip::address::from_string(ip), port);

    try {
        ledman::client server;
        server.start(endpoint);
    } catch (std::exception &e) {
        ledman::logger::error("[server] " + std::string(e.what()));
        return 1;
    }

    return 0;
}
