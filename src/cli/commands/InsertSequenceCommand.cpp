#include "InsertSequenceCommand.h"
#include "Output.h"
#include "MutableSequence.h"
#include "Formatting.h"

namespace cli {

    template <typename... Ts>
    InsertSequenceCommand<Ts...>::InsertSequenceCommand() : Command<Ts...>("insertAt") {}

    template <typename... Ts>
    void InsertSequenceCommand<Ts...>::execute(const std::vector<std::string>& args,
                                               std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) {
        if (args.size() != 3) {
            output::error("Error: Invalid arguments.", "Usage: insertAt <sequence_name> <index> <element>");
            return;
        }

        std::string seqName = args[0];
        int index;

        try {
            index = std::stoi(args[1]);
        } catch (const std::invalid_argument&) {
            output::error("Error: Invalid index format.");
            return;
        }

        auto element = this->parseArgument("element", args, 2);

        MutableSequence<std::variant<Ts...>>* sequence = this->findSequence(seqName, sequences);
        if (sequence == nullptr) {
            return;
        }

        if (index < 0 || index > sequence->getLength()) {
            output::error("Error: Index out of range.");
            return;
        }
        sequence->insertAt(index, element);
        output::success("Element inserted.");
    }

    template <typename... Ts>
    std::string InsertSequenceCommand<Ts...>::getDescription() const {
        return formatting::BOLD + "Description:" + formatting::RESET + " Inserts an element into a mutable sequence at the specified index.\n"
               + formatting::BOLD + "Usage:" + formatting::RESET + " insertAt <sequence_name> <index> <element>\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " insertAt myArray 2 10";
    }

    template class InsertSequenceCommand<int, double, std::string>;
}