#pragma once

#include "utils/location.hh"
#include "utils/message.hh"

namespace ppc::comp::tree::lex {
    struct token_t {
        enum kind_t {
            NONE,
            IDENTIFIER,
            OPERATOR,
            BIN_LITERAL,
            OCT_LITERAL,
            DEC_LITERAL,
            HEX_LITERAL,
            FLOAT_LITERAL,
            STRING_LITERAL,
            CHAR_LITERAL,
        } type;

        std::string data;
        ppc::location_t location;

        static std::vector<token_t> parse_file(ppc::messages::msg_stack_t &msg_stack, const std::string &filename, std::istream &f);
        static std::vector<token_t> parse_many(ppc::messages::msg_stack_t &msg_stack, const std::string &filename, const std::string &src);
    };
}
