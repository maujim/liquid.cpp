#include "liquid/greeting.hpp"

#include <iostream>
#include <string_view>

int main(const int argc, const char* argv[]) {
    const std::string_view name = argc > 1 ? argv[1] : "world";
    std::cout << liquid::greeting(name) << '\n';
    return 0;
}
