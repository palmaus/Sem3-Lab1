#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>

#include "Command.h"

namespace cli {

    template <typename... Ts>
    class Menu : public Command<Ts...> {
    public:
        Menu() : Command<Ts...>("") {}
        explicit Menu(const std::string& name, const std::string& description = "")
                : Command<Ts...>(name), description_(description) {}
        ~Menu() = default;

        // Изменено: принимаем указатель на команду
        void addCommand(std::string commandName, Command<Ts...>* command);

        // Переопределяем execute для навигации по меню
        void execute(const std::vector<std::string>& args,
                    std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) override;

        const std::unordered_map<std::string, Command<Ts...> *>& getCommands() const {
            return commands_;
        }

        // Переопределяем getDescription для вывода информации о меню
        std::string getDescription() const override;

    private:
        std::string description_;
        std::unordered_map<std::string, Command<Ts...>*> commands_; // Теперь храним указатель на Command
    };

} // namespace cli