#pragma once

#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "internal/logging/logger.hpp"

namespace internal {
namespace network {

using service_ptr = boost::shared_ptr<boost::asio::io_service>;

/**
 * asynchronous speaker class
 * defines the default handlers for the read/sent data from tcp connected clients
 * defines the methods to asynchronously read/write the data
 * 'Handler' template intended for user-specific data handling, must have 'std::string operator()(const std::string &)'
 * where return value is sent to server/client
 */
template <typename Handler> /* TODO: check with sfinae that it has 'std::string operator()(const std::string &)' */
class speaker : public boost::enable_shared_from_this<speaker<Handler>> {
private:
    template <typename DataHandler>
    friend class acceptor;
    template <typename DataHandler>
    friend class connector;

public:
    speaker(service_ptr ios)
        : _remote(*ios) {}

    void read_async() {
        _remote.async_read_some(
            boost::asio::buffer(_remote_data, _max_data_length),
            boost::bind(&speaker::on_read, this->shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    void write_async(std::string &message) { /* FIX: make const if '\n' not needed */
        message.push_back('\n');
        _remote.async_write_some(
            boost::asio::buffer(message),
            boost::bind(&speaker::on_write, this->shared_from_this(),
                        boost::asio::placeholders::error));
    }

    boost::asio::ip::tcp::endpoint get_endpoint() {
        return _remote.remote_endpoint();
    }

    virtual ~speaker() {
        close();
    }

protected:
    boost::asio::ip::tcp::socket _remote;
    /* synchronizing destruction */
    boost::mutex _mutex;
    /* handler for received data */
    Handler _handler;

    enum { _max_data_length = 8192 };
    char _remote_data[_max_data_length];

    void on_read(const boost::system::error_code &error,
                 const size_t &bytes_transferred) {

        if (!error) {
            std::string request(_remote_data);
            request.pop_back(); /* FIX: remove '\n' from received msg ? */
            logger::info("[speaker] received message: " + request);
            auto result = _handler(request);
            write_async(result);
        } else {
            logger::error("[speaker] " + error.message());
            close();
        }
    }

    void on_write(const boost::system::error_code &error) {
        if (!error) {
            read_async();
        } else {
            logger::error("[speaker] " + error.message());
            close();
        }
    }

    void close() {
        boost::mutex::scoped_lock lock(_mutex);

        if (_remote.is_open()) {
            _remote.close();
        }
    }
};

} /* namespace network*/
} /* namespace internal */
