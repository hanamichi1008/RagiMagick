#pragma once
#include <map>
#include <string>
#include <string_view>
#include <memory>
#include "ragii/include/text/text.h"

namespace RagiMagick {

    class CommandLine
    {
    public:
        static std::unique_ptr<CommandLine> parse(int argc, char** argv)
        {
            auto cmd = std::make_unique<CommandLine>();

            for (int i = 1; i < argc;) {
                auto opt = std::string_view(argv[i]);
                if (opt[0] == '-') {
                    auto value = i + 1 < argc ? argv[i + 1] : "";
                    cmd->m_Opts[argv[i]] = value;
                    i += 2;
                }
                else {
                    cmd->m_Command = std::string_view(argv[i]);
                    i++;
                }
            }

            return cmd;
        }

        inline std::string getCommand() const
        {
            return m_Command;
        }

        inline bool hasOptions() const
        {
            return !m_Opts.empty();
        }

        inline bool containsKey(std::string_view key) const
        {
            return m_Opts.count(key.data()) > 0;
        }

        inline bool hasValue(std::string_view key)
        {
            return !m_Opts[key.data()].empty();
        }

        template<typename T>
        inline T getValue(std::string_view key)
        {
            return ragii::text::str_to_arithmetic<T>(m_Opts[key.data()].c_str());
        }

        template<>
        inline std::string getValue(std::string_view key)
        {
            return m_Opts[key.data()];
        }

        template<>
        inline std::string_view getValue(std::string_view key)
        {
            return m_Opts[key.data()];
        }


    private:
        std::string m_Command;
        std::map<std::string, std::string> m_Opts;
    };

}
