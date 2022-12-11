#pragma once

#include "internal/logging/logger.hpp"
#include "internal/network/server_impl.hpp"

/**
 * incapsulating internals
 */

namespace ledman {

using server = internal::network::server_impl;
namespace logger = internal::logger;

} /* namespace ledman */
