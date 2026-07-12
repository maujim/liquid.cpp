#include "liquid/tokenizer.hpp"

namespace liquid {

std::vector<TokenId> Tokenizer::tokenize(const std::string_view text) const {
    static_cast<void>(text);
    return {0};
}

}  // namespace liquid
