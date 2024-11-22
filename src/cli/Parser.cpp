#include "Parser.h"
#include <sstream>
#include <algorithm>

namespace cli {

    std::pair<std::string, std::vector<std::string>> Parser::parseCommand(const std::string& commandLine) const {
        std::string command;
        std::vector<std::string> args;

        std::istringstream iss(commandLine);
        iss >> command;

        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }

        return std::make_pair(command, args);
    }

} // namespace cli