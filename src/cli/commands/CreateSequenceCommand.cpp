#include "CreateSequenceCommand.h"
#include "Output.h"
#include "MutableListSequence.h"
#include "Formatting.h"

namespace cli {

    template <typename... Ts>
    CreateSequenceCommand<Ts...>::CreateSequenceCommand() : Command<Ts...>("create") {}

    template <typename... Ts>
    void CreateSequenceCommand<Ts...>::execute(const std::vector<std::string>& args,
                                                std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) {
        if (args.size() < 1) {
            output::error("Error: Invalid arguments.",
                "Usage: create <sequence_name> [elements...]");
            return;
        }

        const std::string name = args[0];

        if (sequences.find(name) != sequences.end()) {
            output::error("Error: Sequence with this name already exists.");
            return;
        }

        sequences[name] = new MutableListSequence<std::variant<Ts...>>();

        for (int i = 1; i < args.size(); ++i) {
            try {
                auto element = this->parseArgument("element", args, i);
                sequences[name]->append(element);
            } catch (const std::invalid_argument&) {
                output::error("Error: Invalid element: ", args[i]);
                return;
            }
        }

        output::success("Sequence '" + name + "' created.");
    }

    template <typename... Ts>
    std::string CreateSequenceCommand<Ts...>::getDescription() const {
        return formatting::BOLD + "Description:" + formatting::RESET + " Creates a new mutable list sequence.\n"
               + formatting::BOLD + "Usage:" + formatting::RESET + " create <sequence_name> [elements...]\n"
               + formatting::BOLD + "Example:" + formatting::RESET + " create myList 1 2 3";
    }

    template class CreateSequenceCommand<int, double, std::string>;
}