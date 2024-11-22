#include "GetSequenceElementCommand.h"
#include "Output.h"
#include "Formatting.h"

namespace cli {

    template <typename... Ts>
    GetSequenceElementCommand<Ts...>::GetSequenceElementCommand() : Command<Ts...>("get") {}

    template <typename... Ts>
    void GetSequenceElementCommand<Ts...>::execute(const std::vector<std::string>& args,
                                            std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) {
        if (args.size() < 2) { // Изменено: args.size() < 3, так как args[0] - имя команды
            output::error("Error: Invalid arguments.", "Usage: get <sequence_name> <index> | first | last");
            return;
        }

        std::string seqName = args[0];
        std::string subcommand = args[1];

        if (subcommand == "first") {
            getFirst(seqName, sequences);
        } else if (subcommand == "last") {
            getLast(seqName, sequences);
        } else {
            try {
                int index = std::stoi(subcommand);
                getByIndex(seqName, index, sequences);
            } catch (const std::invalid_argument&) {
                output::error("Error: Invalid index: ", subcommand);
                return;
            }
        }
    }

    template <typename... Ts>
    void GetSequenceElementCommand<Ts...>::getByIndex(const std::string& seqName, int index,
                                              const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const {
        MutableSequence<std::variant<Ts...>>* sequence = this->findSequence(seqName, sequences);
        if (sequence == nullptr) {
            return;
        }
        if (index < 0 || index >= sequence->getLength()) {
            output::error("Error: Index out of range.");
            return;
        }

        std::visit([](auto&& arg){
            if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                output::success(arg);
            } else {
                output::success(std::to_string(arg));
            }
        }, sequence->get(index));
    }

    template <typename... Ts>
    void GetSequenceElementCommand<Ts...>::getFirst(const std::string& seqName,
                                            const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const {
        MutableSequence<std::variant<Ts...>>* sequence = this->findSequence(seqName, sequences);
        if (sequence == nullptr) {
            return;
        }
        if (sequence->getLength() == 0) {
            output::error("Error: Sequence is empty.");
            return;
        }
        std::visit([](auto&& arg){
            if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                output::success(arg);
            } else {
                output::success(std::to_string(arg));
            }
        }, sequence->getFirst());
    }

    template <typename... Ts>
    void GetSequenceElementCommand<Ts...>::getLast(const std::string& seqName,
                                           const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const {
        MutableSequence<std::variant<Ts...>>* sequence = this->findSequence(seqName, sequences);
        if (sequence == nullptr) {
            return;
        }
        if (sequence->getLength() == 0) {
            output::error("Error: Sequence is empty.");
            return;
        }
        std::visit([](auto&& arg){
            if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                output::success(arg);
            } else {
                output::success(std::to_string(arg));
            }
        }, sequence->getLast());
    }

    template <typename... Ts>
    std::string GetSequenceElementCommand<Ts...>::getDescription() const {
        return formatting::BOLD + "Description:" + formatting::RESET + " Gets element(s) from a mutable sequence.\n"
               + formatting::BOLD + "Usage:" + formatting::RESET + " get <sequence_name> <index|first|last>\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " get myArray 2\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " get myArray first\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " get myArray last";
    }

    template class GetSequenceElementCommand<int, double, std::string>;

} // namespace cli