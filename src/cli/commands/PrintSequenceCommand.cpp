#include "PrintSequenceCommand.h"
#include "Output.h"
#include "Formatting.h"
#include <variant>

namespace cli {

    template <typename... Ts>
    PrintSequenceCommand<Ts...>::PrintSequenceCommand() : Command<Ts...>("print") {}

    template <typename... Ts>
    void PrintSequenceCommand<Ts...>::execute(const std::vector<std::string>& args,
                                               std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) {
        if (args.size() != 1) {
            output::error("Error: Invalid arguments.", "Usage: print <sequence_name>");
            return;
        }

        std::string name = args[0];
        MutableSequence<std::variant<Ts...>>* sequence = this->findSequence(name, sequences);
        if (sequence == nullptr) {
            return;
        }

        for (int i = 0; i < sequence->getLength(); ++i) {
            std::visit([](auto&& arg){std::cout << arg << " ";}, sequence->get(i));
        }
        std::cout << std::endl;
    }

    template <typename... Ts>
    std::string PrintSequenceCommand<Ts...>::getDescription() const {
        return formatting::BOLD + "Description:" + formatting::RESET + " Prints the elements of a mutable sequence to the console.\n"
               + formatting::BOLD + "Usage:" + formatting::RESET + " print <sequence_name>\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " print myArray";
    }

    template class PrintSequenceCommand<int, double, std::string>;
}