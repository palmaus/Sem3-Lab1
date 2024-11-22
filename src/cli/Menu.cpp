#include "Menu.h"
#include "Output.h"

namespace cli {

    template <typename... Ts>
    void Menu<Ts...>::addCommand(std::string commandName, Command<Ts...>* command) {
        commands_[commandName] = command;
    }

    template <typename... Ts>
    void Menu<Ts...>::execute(const std::vector<std::string>& args,
                          std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) {
        if (args.empty()) {
            // Выводим список доступных команд
            output::success("Commands:");
            for (const auto& command : commands_) {
                output::success("  " + command.first + ": " + command.second->getDescription());
            }
            return;
        }

        std::string commandName = args[0];
        std::vector<std::string> commandArgs(args.begin() + 1, args.end());


        auto commandIt = commands_.find(commandName);
        if (commandIt != commands_.end()) {
            commandIt->second->execute(commandArgs, sequences);
            return;
        }

        output::error(" Use 'help' to see available commands. Unknown command: ", commandName);
    }

    template <typename... Ts>
    std::string Menu<Ts...>::getDescription() const {
        return description_;
    }

    template class Menu<int, double, std::string>;

} // namespace cli