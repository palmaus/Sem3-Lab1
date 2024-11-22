#include "ConcatSequenceCommand.h"
#include "Output.h"
#include "MutableListSequence.h"
#include "Formatting.h"

namespace cli {

    template <typename... Ts>
    ConcatSequenceCommand<Ts...>::ConcatSequenceCommand() : Command<Ts...>("concat") {}

    template <typename... Ts>
    void ConcatSequenceCommand<Ts...>::execute(const std::vector<std::string>& args,
                                               std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) {
        if (args.size() != 3) {
            output::error("Error: Invalid arguments.",
                          "Usage: concat <first_sequence_name> <second_sequence_name> <new_sequence_name>");
            return;
        }

        const std::string& firstSeqName = args[0];
        const std::string& secondSeqName = args[1];
        const std::string& newSeqName = args[2];

        MutableSequence<std::variant<Ts...>>* firstSequence = this->findSequence(firstSeqName, sequences);
        if (firstSequence == nullptr) {
            return;
        }
        MutableSequence<std::variant<Ts...>>* secondSequence = this->findSequence(secondSeqName, sequences);
        if (secondSequence == nullptr) {
            return;
        }

        if (sequences.find(newSeqName) != sequences.end()) {
            output::error("Error: Sequence with name '" + newSeqName + "' already exists.");
            return;
        }

        sequences[newSeqName] = new MutableListSequence<std::variant<Ts...>>();

        MutableSequence<std::variant<Ts...>>* newSequence = sequences[newSeqName];

        // Копирование элементов
        for (int i = 0; i < firstSequence->getLength(); ++i) {
            newSequence->append(firstSequence->get(i));
        }
        for (int i = 0; i < secondSequence->getLength(); ++i) {
            newSequence->append(secondSequence->get(i));
        }

        output::success("Sequences concatenated into '" + newSeqName + "'.");
    }

    template <typename... Ts>
    std::string ConcatSequenceCommand<Ts...>::getDescription() const {
        return formatting::BOLD + "Description:" + formatting::RESET + " Concatenates two mutable sequences into a new sequence.\n"
               + formatting::BOLD + "Usage:" + formatting::RESET + " concat <first_sequence_name> <second_sequence_name> <new_sequence_name>\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " concat myArray1 myArray2 myNewArray";
    }

    template class ConcatSequenceCommand<int, double, std::string>;
}