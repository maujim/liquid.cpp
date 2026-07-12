#include "liquid/greeting.hpp"

namespace liquid {

std::string greeting(const std::string_view name) {
    return "Hello, " + std::string{name} + "!";
}

}  // namespace liquid
