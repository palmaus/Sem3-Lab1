#include "AppendSequenceCommand.h"
#include "MutableSequence.h"
#include "Output.h"
#include "Formatting.h"

namespace cli {

    template <typename... Ts>
    AppendSequenceCommand<Ts...>::AppendSequenceCommand() : Command<Ts...>("append") {}

    template <typename... Ts>
    void AppendSequenceCommand<Ts...>::execute(const std::vector<std::string>& args,
                                               std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) {
        if (args.size() != 2) {
            output::error("Invalid arguments.", "Usage: append <sequence_name> <element>");
            return;
        }

        std::string seqName = args[0];
        auto element = this->parseArgument(args[1], args, 1);

        MutableSequence<std::variant<Ts...>>* sequence = this->findSequence(seqName, sequences);
        if (sequence == nullptr) {
            return;
        }

        sequence->append(element);
        output::success("Element appended.");
    }

    template <typename... Ts>
    std::string AppendSequenceCommand<Ts...>::getDescription() const {
        return formatting::BOLD + "Description:" + formatting::RESET + " Appends an element to a mutable sequence.\n"
               + formatting::BOLD + "Usage:" + formatting::RESET + " append <sequence_name> <element>\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " append myArray 5";
    }

    template class AppendSequenceCommand<int, double, std::string>;
}