#include "liquid/greeting.hpp"

#include <iostream>

int main() {
    const auto actual = liquid::greeting("C++");
    const auto expected = std::string{"Hello, C++!"};

    if (actual != expected) {
        std::cerr << "Expected: " << expected << "\nActual:   " << actual << '\n';
        return 1;
    }

    return 0;
}
