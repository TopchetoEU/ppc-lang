#pragma once

#include "lang/common.hh"
#include "treeifier/tokenizer.hh"
#include "utils/data.hh"
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std::string_literals;
using namespace ppc;
using namespace ppc::lang;
using namespace ppc::messages;

namespace ppc::tree::constr {
    struct ast_ctx_t;

    struct glob_t {
        loc_nmsp_t nmsp;
        std::vector<loc_nmsp_t> imports;

        #ifdef PROFILE_debug
        void print() const {
            std::cout << "Namespace: " << nmsp.to_string() << "\n";
            std::cout << "Imports:\n";
            for (auto &imp : imports) {
                std::cout << " - " << imp.to_string() << "\n";
            }
        }
        #endif
    };
    struct exp_t: public named_t {
    };

    template <class ParserT, class GlobT = glob_t>
    class parser_t {
    public:
        virtual bool operator()(ast_ctx_t &ctx, size_t &res_i, ParserT &out) const = 0;
        virtual bool simplify(ast_ctx_t &ctx, GlobT &glob, ParserT &val) const = 0;
    };


    struct ast_ctx_t {
    public:
        msg_stack_t &messages;
        std::vector<token_t> &tokens;
        loc_nmsp_t nmsp;

        ast_ctx_t &operator=(const ast_ctx_t &other) = delete;
        ast_ctx_t(msg_stack_t &messages, std::vector<token_t> &tokens):
            messages(messages), tokens(tokens) { }
    };

    bool parse_identifier(ast_ctx_t &ctx, size_t &res_i, located_t<std::string> &out);
    bool parse_nmsp(ast_ctx_t &ctx, size_t &res_i, loc_nmsp_t &out);
    bool parse_nmsp_id(ast_ctx_t &ctx, size_t &res_i, nmsp_t nmsp);

    class exp_parser_t {
    public:
        bool operator()(ast_ctx_t &ctx, glob_t &out) const;
    };
    class glob_parser_t {
    public:
        bool operator()(ast_ctx_t &ctx, glob_t &out) const;
    };

    // parser_func_t parse_glob, parse_nmsp, parse_identifier, parse_type, parse_exp, parse_stat_exp;
    // parser_func_t parse_func, parse_field, parse_export, parse_struct;
    // parser_func_t parse_if, parse_while, parse_return, parse_break, parse_continue, parse_stat_comp;
    // parser_func_t parse_exp_var, parse_exp_str_lit, parse_exp_int_lit, parse_exp_float_lit;
}