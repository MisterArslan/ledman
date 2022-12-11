#pragma once

#include <string>

namespace internal {
namespace base {

enum LEDColor {
    unknown,
    red,
    green,
    blue
};

std::string ledcolor_to_string(LEDColor color) {
    switch (color) {
    case LEDColor::red:
        return "red";
    case LEDColor::green:
        return "green";
    case LEDColor::blue:
        return "blue";
    default:
        return {};
    }
};

LEDColor string_to_ledcolor(const std::string &color) {
    if (color == "red") {
        return LEDColor::red;
    } else if (color == "green") {
        return LEDColor::green;
    } else if (color == "blue") {
        return LEDColor::blue;
    } else {
        return LEDColor::unknown;
    }
}

} /* namespace base */
} /* namespace internal */