#pragma once

#include <vector>
#include "utils/message.hh"
#include "data.hh"

namespace ppc::options {
    enum flag_match_type_t {
        MATCH_WHOLE,
        MATCH_PREFIX,
    };

    struct parser_t;

    struct flag_t {
        std::string name;
        std::string shorthands;
        std::string description;
        flag_match_type_t match_type;
        void (*execute)(parser_t &parser, const std::string &option, messages::msg_stack_t &global_stack);
    };

    struct parser_t {
    private:
        std::vector<flag_t> flags;
    public:
        void add_flag(const flag_t &flag);
        void clear_flags();

        auto begin() { return flags.begin(); }
        auto end() { return flags.end(); }

        bool parse(const std::string &option, messages::msg_stack_t &msg_stack, data::map_t &conf);
    };
}
