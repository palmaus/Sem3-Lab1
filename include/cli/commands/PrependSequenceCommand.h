#pragma once

#include "Command.h"

namespace cli {

    template <typename... Ts>
    class PrependSequenceCommand : public Command<Ts...> {
    public:
        PrependSequenceCommand();

        void execute(const std::vector<std::string>& args,
                     std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) override;

        std::string getDescription() const override;
    };

} // namespace cli