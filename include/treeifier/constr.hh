#pragma once

#include "utils/message.hh"
#include "lang/common.hh"
#include "treeifier/tokenizer.hh"
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std::string_literals;

namespace ppc::tree {
    using namespace ppc;
    using namespace ppc::lang;
    using namespace ppc::messages;

    struct parse_ctx_t {
    public:
        msg_stack_t &messages;
        std::vector<token_t> &tokens;
        loc_nmsp_t nmsp;

        parse_ctx_t &operator=(const parse_ctx_t &other) = delete;
        parse_ctx_t(msg_stack_t &messages, std::vector<token_t> &tokens);
    };

    namespace parse {
        bool parse_identifier(parse_ctx_t &ctx, size_t &res_i, located_t<std::string> &out);
        bool parse_nmsp(parse_ctx_t &ctx, size_t &res_i, loc_nmsp_t &out);
        bool parse_nmsp_id(parse_ctx_t &ctx, size_t &res_i, std::set<nmsp_t> imports, nmsp_t nmsp);
    }



    // parser_func_t parse_glob, parse_nmsp, parse_identifier, parse_type, parse_exp, parse_stat_exp;
    // parser_func_t parse_func, parse_field, parse_export, parse_struct;
    // parser_func_t parse_if, parse_while, parse_return, parse_break, parse_continue, parse_stat_comp;
    // parser_func_t parse_exp_var, parse_exp_str_lit, parse_exp_int_lit, parse_exp_float_lit;
}