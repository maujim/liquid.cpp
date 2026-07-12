cmake_minimum_required(VERSION 3.20)

project(liquid VERSION 0.1.0 LANGUAGES CXX)

option(LIQUID_BUILD_TESTS "Build the test suite" ON)

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(liquid_lib
    src/greeting.cpp
)

target_include_directories(liquid_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(liquid_lib PUBLIC cxx_std_20)
target_link_libraries(liquid_lib PUBLIC nlohmann_json::nlohmann_json)

if(MSVC)
    target_compile_options(liquid_lib PRIVATE /W4 /permissive-)
else()
    target_compile_options(liquid_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(liquid src/main.cpp)
target_link_libraries(liquid PRIVATE liquid_lib)

if(LIQUID_BUILD_TESTS)
    include(CTest)
    add_executable(liquid_tests tests/test_greeting.cpp)
    target_link_libraries(liquid_tests PRIVATE liquid_lib)
    add_test(NAME liquid_tests COMMAND liquid_tests)
endif()
