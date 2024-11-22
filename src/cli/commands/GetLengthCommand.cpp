#include "GetLengthCommand.h"
#include "Output.h"
#include "Formatting.h"

namespace cli {

    template <typename... Ts>
    GetLengthCommand<Ts...>::GetLengthCommand() : Command<Ts...>("length") {}

    template <typename... Ts>
    void GetLengthCommand<Ts...>::execute(const std::vector<std::string>& args,
                                           std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) {
        if (args.size() != 1) {
            output::error("Error: Invalid arguments.", "Usage: length <sequence_name>");
            return;
        }

        std::string seqName = args[0];
        MutableSequence<std::variant<Ts...>>* sequence = this->findSequence(seqName, sequences);
        if (sequence == nullptr) {
            return;
        }
        output::success(std::to_string(sequence->getLength()));
    }

    template <typename... Ts>
    std::string GetLengthCommand<Ts...>::getDescription() const {
        return formatting::BOLD + "Description:" + formatting::RESET + " Gets the length of a mutable sequence.\n"
               + formatting::BOLD + "Usage:" + formatting::RESET + " length <sequence_name>\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " length myArray";
    }

    template class GetLengthCommand<int, double, std::string>;
}