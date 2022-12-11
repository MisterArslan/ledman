#pragma once

#include "internal/logging/logger.hpp"
#include "internal/network/client_impl.hpp"

/**
 * incapsulating internals
 */

namespace ledman {

using client = internal::network::client_impl;
namespace logger = internal::logger;

} /* namespace ledman */
