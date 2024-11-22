#include "Session.h"
#include "Parser.h"
#include "Output.h"
#include "Formatting.h"
#include <iostream>
#include <algorithm>
#include <cctype>


namespace cli {

    template <typename... Ts>
    Session<Ts...>::Session(Menu<Ts...>& menu) : menu_(menu)  {}

    template <typename... Ts>
    Session<Ts...>::~Session() {
        for (auto& pair : sequences_) {
            delete pair.second;
        }
    }

    template <typename... Ts>
    void Session<Ts...>::run() {
        std::string commandLine;

        while (true) {
            std::cout << ">> ";
            std::getline(std::cin, commandLine);

            if (commandLine == "exit") {
                break;
            }

            processCommand(commandLine);
        }
    }

    template <typename... Ts>
    void Session<Ts...>::processCommand(const std::string& commandLine) {
        Parser parser;
        auto [commandName, args] = parser.parseCommand(commandLine);

        if (commandName == "help") {
            if (args.empty()) {
                showHelp();
            } else if (args.size() == 1) {
                showHelp(args[0]);
            } else {
                output::error("Error: Invalid arguments for 'help'. Usage: help [command]");
            }
            return;
        }

        args.insert(args.begin(), commandName);

        // Вызываем execute меню для обработки команды
        menu_.execute(args, sequences_);
    }

    template <typename... Ts>
    void Session<Ts...>::showHelp() const {
        output::success(formatting::BOLD + "Available commands:" + formatting::RESET);
        const auto& commands = menu_.getCommands();

        for (const auto& commandPair : commands) {
            output::success("  " + formatting::GREEN + commandPair.first + formatting::RESET + ": " + commandPair.second->getDescription() + "\n");
        }
    }

    template <typename... Ts>
    void Session<Ts...>::showHelp(const std::string& commandName) const {
        const auto& commands = menu_.getCommands();

        auto commandIt = commands.find(commandName);
        if (commandIt != commands.end()) {
            output::success(commandIt->second->getDescription());
        } else {
            output::error("Error: Unknown command: ", commandName);
        }
    }

    template class Session<int, double, std::string>;

} // namespace cli