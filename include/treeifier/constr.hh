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
        loc_namespace_name_t nmsp;
        std::vector<loc_namespace_name_t> imports;
    };

    template <class T, class GlobT = glob_t>
    class parser_t {
    public:
        virtual bool operator()(ast_ctx_t &ctx, size_t &res_i, T &out) const = 0;
        virtual bool simplify(ast_ctx_t &ctx, GlobT &glob, T &val) const = 0;
#ifdef PROFILE_debug
        virtual void print(const T &val) {
            std::cout << "(unknown)";
        }
#endif
    };


    struct ast_ctx_t {
    public:
        msg_stack_t &messages;
        std::vector<token_t> &tokens;
        loc_namespace_name_t nmsp;

        ast_ctx_t &operator=(const ast_ctx_t &other) = delete;

        template <class T>
        bool parse(const parser_t<T> &parser, size_t &i, T &out) {
            return parser(*this, i, out);
        }

        template <class T>
        static T parse(const parser_t<T> &glob, msg_stack_t &messages, std::vector<token_t> &tokens) {
            ast_ctx_t ctx(messages, tokens);
            T res;
            size_t i = 0;

            if (!ctx.parse(glob, i, res)) throw message_t::error("Failed to compile.");
            return res;
        }

        ast_ctx_t(msg_stack_t &messages, std::vector<token_t> &tokens);
    };

    template <class T>
    class inspoint_t {
    private:
        std::map<lang::namespace_name_t, std::string> named_parsers;
        std::set<std::string> unnamed_parsers;
        std::map<std::string, T *> parsers;
    public:
        inspoint_t &replace(const std::string &name, const T &parser) {
            auto it = parsers.find(name);

            if (parsers.find(name) == parsers.end()) {
                throw "The parser '" + name + "' isn't in the group.";
            }

            it->second = parser;

            return *this;
        }
        inspoint_t &add(const std::string &name, const T &parser);
        inspoint_t &add(const std::string &name, const lang::namespace_name_t &identifier, const T &parser);

        bool operator()(ast_ctx_t &ctx, size_t &i, data::map_t &out) const;
    };

    // parser_func_t parse_glob, parse_nmsp, parse_identifier, parse_type, parse_exp, parse_stat_exp;
    // parser_func_t parse_func, parse_field, parse_export, parse_struct;
    // parser_func_t parse_if, parse_while, parse_return, parse_break, parse_continue, parse_stat_comp;
    // parser_func_t parse_exp_var, parse_exp_str_lit, parse_exp_int_lit, parse_exp_float_lit;
}