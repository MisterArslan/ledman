#pragma once

#include <memory>

#include <spdlog/spdlog.h>

namespace internal {
namespace logger {

namespace {
static std::shared_ptr<spdlog::logger> _logger = spdlog::stdout_color_mt("console");
} /* anonymous namespace */

static void info(const std::string &msg) {
    _logger->info(msg);
}

static void warn(const std::string &msg) {
    _logger->warn(msg);
}

static void error(const std::string &msg) {
    _logger->error(msg);
}

static void debug(const std::string &msg) {
    _logger->debug(msg);
}

} /* namespace logger */
} /* namespace internal */
