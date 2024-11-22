#pragma once

#include <string>

namespace cli {
    namespace formatting {

        // Цвета
        const std::string RESET = "\033[0m";
        const std::string RED = "\033[31m";
        const std::string GREEN = "\033[32m";

        // Стили
        const std::string BOLD = "\033[1m";
        const std::string UNDERLINE = "\033[4m";

    } // namespace formatting
} // namespace cli