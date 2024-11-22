#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include "Command.h"
#include "Menu.h"

namespace cli {

    template <typename... Ts>
    class MenuBuilder {
    private:
        std::string name;
        std::string description;
        std::unordered_map<std::string, Command<Ts...>*> commands;

    public:
        explicit MenuBuilder(std::string  name, std::string  description = "") : name(std::move(name)), description(std::move(description)) {}

        // Метод для добавления команды
        MenuBuilder& addCommand(const std::string& commandName, Command<Ts...>* command) {
            commands[commandName] = std::move(command); // Используем std::move для передачи владения
            return *this;
        }

        // Метод для построения Menu
        Menu<Ts...> build() {
            Menu<Ts...> menu(name, description);
            for (const auto& commandPair : commands) {
                menu.addCommand(commandPair.first, commandPair.second);
            }
            return menu;
        }
    };

} // namespace cli