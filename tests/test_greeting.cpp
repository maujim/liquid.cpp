#include "liquid/greeting.hpp"
#include "liquid/tokenizer.hpp"

#include <iostream>

int main() {
    const auto actual = liquid::greeting("C++");
    const auto expected = std::string{"Hello, C++!"};

    if (actual != expected) {
        std::cerr << "Expected: " << expected << "\nActual:   " << actual << '\n';
        return 1;
    }

    const liquid::Tokenizer tokenizer;
    const auto tokens = tokenizer.tokenize("some text");
    if (tokens != std::vector<liquid::TokenId>{0}) {
        std::cerr << "Expected the placeholder tokenizer to return token ID 0.\n";
        return 1;
    }

    return 0;
}
