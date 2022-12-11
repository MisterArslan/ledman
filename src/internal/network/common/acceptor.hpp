#pragma once

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include "internal/logging/logger.hpp"
#include "speaker.hpp"

namespace internal {
namespace network {

/**
 * asynchronous acceptor class
 * receives tcp connections from clients into 'speaker' object instances
 */
template <typename DataHandler>
class acceptor {
protected:
    using speaker_ptr = boost::shared_ptr<speaker<DataHandler>>;

public:
    acceptor(service_ptr io_service, const boost::asio::ip::tcp::endpoint &endpoint)
        : _ios(io_service), _acceptor(*_ios, endpoint) {}

    void accept_connections() {
        try {
            _speaker = boost::make_shared<speaker<DataHandler>>(_ios);

            _acceptor.async_accept(_speaker->_remote,
                                   boost::bind(&acceptor::on_connect, this,
                                               boost::asio::placeholders::error));
        } catch (std::exception &e) {
            logger::error("[acceptor] " + std::string(e.what()));
        }
    }

protected:
    /* io_service */
    service_ptr _ios;
    /* client acceptor */
    boost::asio::ip::tcp::acceptor _acceptor;
    /* client(s) */
    speaker_ptr _speaker;

    void on_connect(const boost::system::error_code &error) {
        if (!error) {
            auto client_ep = _speaker->get_endpoint();
            logger::info("[acceptor] got new client: " + client_ep.address().to_string());

            _speaker->read_async();
            accept_connections();
        } else {
            logger::error("[acceptor] " + std::string(error.message()));
        }
    }
};

} /* namespace network*/
} /* namespace internal */
