#include "liquid/greeting.hpp"

#include <nlohmann/json.hpp>

#include <array>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string_view>
#include <vector>

namespace {

namespace fs = std::filesystem;

std::string human_size(const std::uintmax_t bytes) {
    constexpr std::uintmax_t unit = 1024;
    constexpr std::string_view units[] = {"B", "KiB", "MiB", "GiB", "TiB"};

    double size = static_cast<double>(bytes);
    std::size_t unit_index = 0;
    while (size >= unit && unit_index + 1 < std::size(units)) {
        size /= unit;
        ++unit_index;
    }

    std::ostringstream output;
    output << std::fixed << std::setprecision(unit_index == 0 ? 0 : 2)
           << size << ' ' << units[unit_index];
    return output.str();
}

std::optional<std::string> read_header(const fs::path& weights) {
    std::ifstream input{weights, std::ios::binary};
    if (!input) {
        return std::nullopt;
    }

    std::array<unsigned char, 8> length_bytes{};
    if (!input.read(reinterpret_cast<char*>(length_bytes.data()), length_bytes.size())) {
        return std::nullopt;
    }

    std::uint64_t header_length = 0;
    for (std::size_t i = 0; i < length_bytes.size(); ++i) {
        header_length |= static_cast<std::uint64_t>(length_bytes[i]) << (i * 8);
    }

    if (header_length > static_cast<std::uint64_t>(fs::file_size(weights) - length_bytes.size())) {
        return std::nullopt;
    }

    std::string header(static_cast<std::size_t>(header_length), '\0');
    if (!input.read(header.data(), static_cast<std::streamsize>(header.size()))) {
        return std::nullopt;
    }
    return header;
}

std::optional<fs::path> find_model() {
    const char* home = std::getenv("HOME");
    if (home == nullptr) {
        return std::nullopt;
    }

    const fs::path cache = fs::path{home} / ".cache" / "huggingface" / "hub";
    const fs::path model_root = cache / "models--LiquidAI--LFM2.5-350M-MLX-bf16";
    const fs::path refs_main = model_root / "refs" / "main";

    std::ifstream ref_file{refs_main};
    std::string revision;
    if (ref_file >> revision) {
        const fs::path snapshot = model_root / "snapshots" / revision;
        if (fs::is_regular_file(snapshot / "model.safetensors")) {
            return snapshot;
        }
    }

    const fs::path snapshots = model_root / "snapshots";
    if (!fs::is_directory(snapshots)) {
        return std::nullopt;
    }
    for (const auto& entry : fs::directory_iterator{snapshots}) {
        if (fs::is_regular_file(entry.path() / "model.safetensors")) {
            return entry.path();
        }
    }
    return std::nullopt;
}

}  // namespace

int main(const int argc, const char* argv[]) {
    std::cout << R"FIGLET( _ _             _     _                   
| (_) __ _ _   _(_) __| |  ___ _ __  _ __  
| | |/ _` | | | | |/ _` | / __| '_ \| '_ \ 
| | | (_| | |_| | | (_| || (__| |_) | |_) |
|_|_|\__, |\__,_|_|\__,_(_)___| .__/| .__/ 
        |_|                   |_|   |_|    
)FIGLET";

    constexpr std::string_view supported_model = "LiquidAI/LFM2.5-350M-MLX-bf16";
    std::string_view model_id;
    if (argc == 2 && std::string_view{argv[1]} == "--default") {
        model_id = supported_model;
    } else if (argc >= 2 && std::string_view{argv[1]} == "-hf") {
        if (argc < 3) {
            std::cerr << "-hf requires a model identifier.\n";
            return 1;
        }
        model_id = argv[2];
        if (model_id != supported_model) {
            std::cerr << "The model '" << model_id << "' is not currently supported.\n";
            return 1;
        }
    } else {
        std::cerr << "Usage: liquid --default\n"
                  << "       liquid -hf " << supported_model << '\n';
        return 1;
    }

    std::cout << "Model: " << model_id << '\n';

    const auto model = find_model();
    if (!model) {
        std::cerr << "LFM2.5-350M MLX/BF16 model was not found in ~/.cache/huggingface.\n";
        return 1;
    }

    const fs::path weights = *model / "model.safetensors";
    const auto bytes = fs::file_size(weights);
    std::cout << "Found model at: " << model->string() << '\n';
    std::cout << "loading into memory... (" << bytes << " bytes, "
              << human_size(bytes) << ")\n";

    const auto header = read_header(weights);
    if (!header) {
        std::cerr << "Failed to read the safetensors JSON header.\n";
        return 1;
    }
    try {
        const auto parsed_header = nlohmann::json::parse(*header);
        std::cout << "Safetensors tensors:\n";
        for (const auto& [name, tensor] : parsed_header.items()) {
            if (name == "__metadata__") {
                continue;
            }
            std::cout << "  " << name << ": shape "
                      << tensor.at("shape").dump() << '\n';
        }
    } catch (const nlohmann::json::parse_error& error) {
        std::cerr << "Failed to parse the safetensors JSON header: " << error.what() << '\n';
        return 1;
    }

    void* memory = std::malloc(bytes);
    if (memory == nullptr) {
        std::cerr << "Failed to allocate " << bytes << " bytes for model weights.\n";
        return 1;
    }
    std::free(memory);
    return 0;
}
