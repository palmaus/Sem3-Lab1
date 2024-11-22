#include "GetSubsequenceCommand.h"
#include "Output.h"
#include "Formatting.h"

namespace cli {

    template <typename... Ts>
    GetSubsequenceCommand<Ts...>::GetSubsequenceCommand() : Command<Ts...>("getSubsequence") {}

    template <typename... Ts>
    void GetSubsequenceCommand<Ts...>::execute(const std::vector<std::string>& args,
                                               std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) {
        if (args.size() != 3) {
            output::error("Error: Invalid arguments.",
                          "Usage: getSubsequence <sequence_name> <start_index> <end_index>");
            return;
        }

        std::string seqName = args[0];
        int startIndex, endIndex;

        try {
            startIndex = std::stoi(args[1]);
            endIndex = std::stoi(args[2]);
        } catch (const std::invalid_argument&) {
            output::error("Error: Invalid index format.");
            return;
        }

        MutableSequence<std::variant<Ts...>>* sequence = this->findSequence(seqName, sequences);
        if (sequence == nullptr) {
            return;
        }
        if (startIndex < 0 || endIndex >= sequence->getLength() || startIndex > endIndex) {
            output::error("Error: Invalid start or end index.");
            return;
        }

        MutableSequence<std::variant<Ts...>>* subsequence = sequence->getSubSequence(startIndex, endIndex);
        if (subsequence == nullptr) {
            output::error("Error: Could not cast subsequence to MutableSequence.");
            return;
        }

        for (int i = 0; i < subsequence->getLength(); ++i) {
            std::visit([](auto&& arg){std::cout << arg << " ";}, subsequence->get(i));
        }
        std::cout << std::endl;

        delete subsequence;
    }

    template <typename... Ts>
    std::string GetSubsequenceCommand<Ts...>::getDescription() const {
        return formatting::BOLD + "Description:" + formatting::RESET +  " Gets a subsequence from a mutable sequence.\n"
               + formatting::BOLD + "Usage:" + formatting::RESET + " getSubsequence <sequence_name> <start_index> <end_index> <new_sequence_name>.\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " getSubsequence myArray 0 2 mySubArray";
    }

    template class GetSubsequenceCommand<int, double, std::string>;
}