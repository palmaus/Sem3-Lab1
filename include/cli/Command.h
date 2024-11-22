#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <stdexcept>

#include "MutableSequence.h"
#include "Output.h"

namespace cli {

    template <typename... Ts>
    class Command {
    public:
        explicit Command(std::string name) : name_(name), enabled_(true) {}
        virtual ~Command() = default;

        void enable() { enabled_ = true; }
        void disable() { enabled_ = false; }

        // Метод для выполнения команды
        virtual void execute(const std::vector<std::string>& args,
                            std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) = 0;

        // Метод для получения описания команды
        virtual std::string getDescription() const = 0;

        const std::string& name() const { return name_; }

        // Вспомогательные методы для парсинга аргументов
        std::variant<Ts...> parseArgument(const std::string& argName, const std::vector<std::string>& args, int argIndex) const;

        // Метод для поиска последовательности
        MutableSequence<std::variant<Ts...>>* findSequence(const std::string& seqName,
                                  const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const;
    protected:
        bool isEnabled() const { return enabled_; }

    private:
        std::string name_;
        bool enabled_;
    };

    // Реализация вспомогательных методов для Command
    template <typename... Ts>
    std::variant<Ts...> Command<Ts...>::parseArgument(const std::string& argName, const std::vector<std::string>& args, int argIndex) const {
        if (argIndex >= args.size()) {
            throw std::runtime_error("Missing argument: " + argName);
        }

        std::variant<Ts...> parsedValue;

        // Проверяем наличие десятичной точки
        if (args[argIndex].find('.') != std::string::npos) {
            try {
                parsedValue = std::stod(args[argIndex]); // <-  Если есть точка, то double
            } catch (const std::invalid_argument&) {
                throw std::runtime_error("Invalid double argument: " + argName);
            }
        } else {
            try {
                parsedValue = std::stoi(args[argIndex]); // <- Иначе пробуем int
            } catch (const std::invalid_argument&) {
                parsedValue = args[argIndex]; // <-  Если не удалось - строка
            }
        }

        return parsedValue;
    }

    template<typename... Ts>
    MutableSequence<std::variant<Ts...>>* Command<Ts...>::findSequence(const std::string& seqName,
                                        const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const {
        auto it = sequences.find(seqName);
        if (it != sequences.end()) {
            return it->second;
        }
        output::error("Error: Sequence not found: ", seqName);
        return nullptr;
    }

} // namespace cli