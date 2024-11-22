#pragma once

#include <string>
#include <vector>

namespace cli {

    class Parser {
    public:
        // Разбивает командную строку на команду и аргументы
        std::pair<std::string, std::vector<std::string>> parseCommand(const std::string& commandLine) const;
    };

} // namespace cli