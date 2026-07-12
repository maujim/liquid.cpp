#pragma once

#include <string>
#include <string_view>

namespace liquid {

[[nodiscard]] std::string greeting(std::string_view name);

}  // namespace liquid
