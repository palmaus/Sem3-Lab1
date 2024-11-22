#include "TryGetSequenceElementCommand.h"
#include "Output.h"
#include "Formatting.h"
#include "Option.h"

namespace cli {

    template <typename... Ts>
    TryGetSequenceElementCommand<Ts...>::TryGetSequenceElementCommand() : Command<Ts...>("tryGet") {}

    template <typename... Ts>
    void TryGetSequenceElementCommand<Ts...>::execute(const std::vector<std::string>& args,
                                                      std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) {
        if (args.size() < 2) {
            output::error("Error: Invalid arguments.", "Usage: tryGet <sequence_name> <index> | first | last");
            return;
        }

        std::string seqName = args[0];
        std::string subcommand = args[1];

        if (subcommand == "first") {
            tryGetFirst(seqName, sequences);
        } else if (subcommand == "last") {
            tryGetLast(seqName, sequences);
        } else {
            try {
                int index = std::stoi(subcommand);
                tryGetByIndex(seqName, index, sequences);
            } catch (const std::invalid_argument&) {
                output::error("Error: Invalid index: ", subcommand);
                return;
            }
        }
    }

    template <typename... Ts>
    void TryGetSequenceElementCommand<Ts...>::tryGetByIndex(const std::string& seqName, int index,
                                                            const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const {
        MutableSequence<std::variant<Ts...>>* sequence = this->findSequence(seqName, sequences);
        if (sequence == nullptr) {
            return; // Выход из функции, если последовательность не найдена
        }
        Option<std::variant<Ts...>> element = sequence->tryGet(index);
        if (element.isSome()) {
            std::visit([](auto&& arg){
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                    output::success(arg);
                } else {
                    output::success(std::to_string(arg));
                }
            }, element.getValue());
        } else {
            output::success("None");
        }
    }

    template <typename... Ts>
    void TryGetSequenceElementCommand<Ts...>::tryGetFirst(const std::string& seqName,
                                                          const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const {
        MutableSequence<std::variant<Ts...>>* sequence = this->findSequence(seqName, sequences);
        if (sequence == nullptr) {
            return; // Выход из функции, если последовательность не найдена
        }
        Option<std::variant<Ts...>> element = sequence->tryGetFirst();
        if (element.isSome()) {
            std::visit([](auto&& arg){
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                    output::success(arg);
                } else {
                    output::success(std::to_string(arg));
                }
            }, element.getValue());
        } else {
            output::success("None");
        }
    }

    template <typename... Ts>
    void TryGetSequenceElementCommand<Ts...>::tryGetLast(const std::string& seqName,
                                                         const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const {
        MutableSequence<std::variant<Ts...>>* sequence = this->findSequence(seqName, sequences);
        if (sequence == nullptr) {
            return; // Выход из функции, если последовательность не найдена
        }
        Option<std::variant<Ts...>> element = sequence->tryGetLast();
        if (element.isSome()) {
            std::visit([](auto&& arg){
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                    output::success(arg);
                } else {
                    output::success(std::to_string(arg));
                }
            }, element.getValue());
        } else {
            output::success("None");
        }
    }

    template <typename... Ts>
    std::string TryGetSequenceElementCommand<Ts...>::getDescription() const {
        return formatting::BOLD + "Description:" + formatting::RESET + " Try to get element(s) from a mutable sequence. Returns 'None' if the element is not found.\n"
               + formatting::BOLD + "Usage:" + formatting::RESET + " tryGet <sequence_name> <index|first|last>\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " tryGet myArray 5\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " tryGet myArray first\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " tryGet myArray last";
    }


    template class TryGetSequenceElementCommand<int, double, std::string>;

} // namespace cli