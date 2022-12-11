#pragma once

#include <memory>

#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <boost/make_unique.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "internal/base/ledmanager.hpp"
#include "internal/logging/logger.hpp"
#include "internal/network/common/acceptor.hpp"

namespace internal {
namespace network {

class DataHandler {
public:
    std::string operator()(const std::string &request) {
        boost::mutex::scoped_lock lock(_mutex);
        return internal::base::execute_by_string(led, request);
    }

private:
    static internal::base::LED led;
    /* synchronizing work with LED */
    boost::mutex _mutex;
};
internal::base::LED DataHandler::led;

/**
 * server_impl class
 * runs the server to handle requests for the led(s)
 */
class server_impl {
private:
    using ios_ptr = boost::shared_ptr<boost::asio::io_service>;
    using acc_type = acceptor<DataHandler>;

public:
    server_impl() : _ios(boost::make_shared<ios_ptr::element_type>()) {}

    void start(boost::asio::ip::tcp::endpoint endpoint) {
        try {
            auto acceptor = boost::make_unique<acc_type>(_ios, endpoint);
            acceptor->accept_connections();

            logger::info("[server] started!");
            _ios->run();
        } catch (std::exception &e) {
            logger::error("[server] " + std::string(e.what()));
            return;
        }
    }

private:
    ios_ptr _ios;
};

} /* namespace network*/
} /* namespace internal */
