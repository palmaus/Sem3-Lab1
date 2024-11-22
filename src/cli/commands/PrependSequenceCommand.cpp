#include "PrependSequenceCommand.h"
#include "MutableSequence.h"
#include "Output.h"
#include "Formatting.h"

namespace cli {

    template <typename... Ts>
    PrependSequenceCommand<Ts...>::PrependSequenceCommand() : Command<Ts...>("prepend") {}

    template <typename... Ts>
    void PrependSequenceCommand<Ts...>::execute(const std::vector<std::string>& args,
                                                std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) {
        if (args.size() != 2) {
            output::error("Error: Invalid arguments.", "Usage: prepend <sequence_name> <element>");
            return;
        }

        std::string seqName = args[0];
        auto element = this->parseArgument("element", args, 1);

        MutableSequence<std::variant<Ts...>>* sequence = this->findSequence(seqName, sequences);
        if (sequence == nullptr) {
            return;
        }
        sequence->prepend(element);
        output::success("Element prepended.");
    }

    template <typename... Ts>
    std::string PrependSequenceCommand<Ts...>::getDescription() const {
        return formatting::BOLD + "Description:" + formatting::RESET + " Prepends an element to a mutable sequence.\n"
               + formatting::BOLD + "Usage:" + formatting::RESET + " prepend <sequence_name> <element>\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " prepend myArray 0";
    }

    template class PrependSequenceCommand<int, double, std::string>;
}