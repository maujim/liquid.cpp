#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

namespace liquid {

using TokenId = std::uint32_t;

class Tokenizer {
  public:
    [[nodiscard]] std::vector<TokenId> tokenize(std::string_view text) const;
};

}  // namespace liquid
