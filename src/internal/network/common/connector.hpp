#pragma once

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include "internal/logging/logger.hpp"
#include "speaker.hpp"

namespace internal {
namespace network {

/**
 * asynchronous connector class
 * connects and communicates with the remote server through TCP
 */
template <typename DataHandler>
class connector {
protected:
    using speaker_ptr = boost::shared_ptr<speaker<DataHandler>>;

public:
    connector(service_ptr ios, const boost::asio::ip::tcp::endpoint &endpoint)
        : _ios(ios), _endpoint(endpoint), _speaker(boost::make_shared<typename speaker_ptr::element_type>(_ios)) {}

    void connect() {
        try {
            _speaker->_remote.async_connect(_endpoint, boost::bind(&connector::on_connect, this,
                                                                   boost::asio::placeholders::error));
        } catch (std::exception &e) {
            logger::error("[connector] " + std::string(e.what()));
        }
    }

    void write(std::string &message) {
        _speaker->write_async(message);
    }

protected:
    /* io service */
    service_ptr _ios;
    /* client */
    speaker_ptr _speaker;
    /* server */
    boost::asio::ip::tcp::endpoint _endpoint;

    void on_connect(const boost::system::error_code &error) {
        if (!error) {
            logger::info("[connector] connected successfully!");
        } else {
            logger::error("[connector] " + std::string(error.message()));
        }
    }
};

} /* namespace network*/
} /* namespace internal */
