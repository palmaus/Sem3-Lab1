#pragma once

#include "Command.h"

namespace cli {

    template <typename... Ts>
    class TryGetSequenceElementCommand : public Command<Ts...> {
    public:
        TryGetSequenceElementCommand();

        void execute(const std::vector<std::string>& args,
                     std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) override;

        std::string getDescription() const override;

    private:
        void tryGetByIndex(const std::string& seqName, int index,
                           const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const;
        void tryGetFirst(const std::string& seqName,
                          const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const;
        void tryGetLast(const std::string& seqName,
                         const std::unordered_map<std::string, MutableSequence<std::variant<Ts...>>*>& sequences) const;
    };

} // namespace cli