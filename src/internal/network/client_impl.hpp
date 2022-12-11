#pragma once

#include <iostream>

#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <boost/make_unique.hpp>
#include <boost/shared_ptr.hpp>

#include "internal/logging/logger.hpp"
#include "internal/network/common/connector.hpp"

namespace internal {
namespace network {

class DataHandler {
public:
    std::string operator()(const std::string &) {
        std::string message;
        logger::info("[client] what to send?");
        std::getline(std::cin, message);
        return message;
    }
};

/**
 * client_impl class
 * runs the client to send requests for the led(s)
 */
class client_impl {
private:
    using ios_ptr = boost::shared_ptr<boost::asio::io_service>;
    using conn_type = connector<DataHandler>;

public:
    client_impl() : _ios(boost::make_shared<ios_ptr::element_type>()) {}

    void start(boost::asio::ip::tcp::endpoint endpoint) {
        try {
            auto connector = boost::make_unique<conn_type>(_ios, endpoint);
            connector->connect();
            logger::info("[client] started!");

            DataHandler handler;
            std::string message = handler({});
            connector->write(message);

            _ios->run();
        } catch (std::exception &e) {
            logger::error("[client] " + std::string(e.what()));
            return;
        }
    }

private:
    ios_ptr _ios;
};

} /* namespace network*/
} /* namespace internal */
