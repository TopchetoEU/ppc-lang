#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include <memory>
#include "compiler/treeifier/tokenizer.hh"
#include "utils/data.hh"
#include "utils/slice.hh"
#include "lang/common.hh"

using namespace std::string_literals;
using namespace ppc;

namespace ppc::comp::tree::ast {
    class constr_parser_t {
    private:
        std::string name;
    public:
        const std::string &name() { return name; }
        virtual bool parse(messages::msg_stack_t &messages, vec_slice_t<tok::token_t> &tokens, data::map_t &out) = 0;
    };

    class group_parser_t : constr_parser_t {
    private:
        struct named_parser {
            constr_parser_t *parser;
            std::string name;
        };
        std::list<constr_parser_t*> parsers;
        std::unordered_map<std::string, constr_parser_t*> insertion_points;
    public:
        void add_insertion_point(constr_parser_t &parser, const std::string &name);
        void add(constr_parser_t &parser);
        void add(const std::string &ins_point, constr_parser_t &parser);

        bool parse(messages::msg_stack_t &messages, data::map_t &out);

        group_parser_t();
    };

    extern const constr_parser_t &glob_parser;
}