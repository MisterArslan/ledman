#pragma once

#include <string>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "internal/logging/logger.hpp"

#include "common.hpp"

namespace internal {
namespace base {

// ---- command ---------- argument --------------- result --------------- description -----------------
// | set_led_state  | on, off            | OK, FAILED                | turn LED on/off                 |
// | get_led_state  |                    | OK, on/off, FAILED        | request LED state               |
// | set_led_color  | red, green, blue   | OK, FAILED                | change LED color                |
// | get_led_color  |                    | OK red/green/blue, FAILED | request LED color               |
// | set_led_rate   | 0..5               | OK, FAILED                | change LED lightning frequency  |
// | get_led_rate   |                    | OK 0..5, FAILED           | request LED lightning frequency |
// -----------------------------------------------------------------------------------------------------

#define DEFAULT_RATE 1
#define DEFAULT_COLOR LEDColor::blue

/**
 * Class for LED management, contains basic state/color/rate read/update operations
 */
class LED {
public:
    LED() : _state(false), _rate(DEFAULT_RATE), _color(DEFAULT_COLOR) {}

    bool set_state(const std::string &state) {
        if (state != "on" && state != "off") {
            logger::warn("[manager] unknown LED state");
            return false;
        }
        return set_state((state == "on") ? true : false);
    }

    bool set_state(const bool state) {
        /* TODO: implement */
        _state = state;
        logger::debug("[manager] LED state set to " + state);
        return true;
    }

    bool get_state(bool &state) {
        /* TODO: implement */
        state = _state;
        return true;
    }

    bool set_color(const std::string &color) {
        auto ledcolor = string_to_ledcolor(color);
        if (ledcolor == LEDColor::unknown) {
            logger::warn("[manager] unknown LED color");
            return false;
        }
        return set_color(ledcolor);
    }

    bool set_color(const LEDColor &color) {
        /* TODO: implement */
        _color = color;
        logger::debug("[manager] LED color set to " + ledcolor_to_string(color));
        return true;
    }

    bool get_color(LEDColor &color) {
        /* TODO: implement */
        color = _color;
        return true;
    }

    bool set_rate(const std::string &rate) {
        auto value = std::stoi(rate);
        if (value < 1 || value > 5) {
            logger::warn("[manager] unknown LED rate");
            return false;
        }
        return set_rate(static_cast<short>(value));
    }

    bool set_rate(const short &rate) {
        /* TODO: implement */
        _rate = rate;
        logger::debug("[manager] LED rate set to " + rate);
        return true;
    }

    bool get_rate(short &rate) {
        /* TODO: implement */
        rate = _rate;
        return true;
    }

private:
    bool _state; /* on|off */
    LEDColor _color;
    short _rate; /* Hz */
};

std::string execute_by_string(LED &led, const std::string &command) {
    std::vector<std::string> tokens;
    boost::split(tokens, command, boost::is_any_of(" "));

    if (tokens.size() == 0) {
        logger::warn("[manager] received unknown command: " + command);
        return {};
    }

    /* set_state */
    if (tokens[0] == "set-led-state") {
        if (tokens.size() < 2) {
            logger::warn("[manager] set_state must have an argument");
            return {};
        }
        bool res = led.set_state(tokens[1]);
        return (res) ? "OK" : "FAILED";
    }
    /* get_state */
    else if (tokens[0] == "get-led-state") {
        bool state;
        bool res = led.get_state(state);
        std::string response = (res) ? "OK" : "FAILED";
        response += (state) ? " on" : " off";
        return response;
    }
    /* set_color */
    else if (tokens[0] == "set-led-color") {
        if (tokens.size() < 2) {
            logger::warn("[manager] set_color must have an argument");
            return {};
        }
        bool res = led.set_color(tokens[1]);
        return (res) ? "OK" : "FAILED";
    }
    /* get_color */
    else if (tokens[0] == "get-led-color") {
        internal::base::LEDColor color;
        bool res = led.get_color(color);
        std::string response = (res) ? "OK" : "FAILED";
        response += " " + internal::base::ledcolor_to_string(color);
        return response;
    }
    /* set_rate */
    else if (tokens[0] == "set-led-rate") {
        if (tokens.size() < 2) {
            logger::warn("[manager] set_rate must have an argument");
            return {};
        }
        bool res = led.set_rate(tokens[1]);
        return (res) ? "OK" : "FAILED";
    }
    /* get_rate */
    else if (tokens[0] == "get-led-rate") {
        short rate;
        bool res = led.get_rate(rate);
        std::string response = (res) ? "OK" : "FAILED";
        response += " " + std::to_string(rate);
        return response;
    }
    /* unknown command */
    else {
        logger::warn("[manager] received unknown command: " + command);
        return {};
    }
}

} /* namespace base */
} /* namespace internal */