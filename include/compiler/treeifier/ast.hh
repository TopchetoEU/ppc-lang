#pragma once

#include <string>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include "compiler/treeifier/tokenizer.hh"
#include "utils/data.hh"
#include "lang/common.hh"

using namespace std::string_literals;
using namespace ppc;
using namespace ppc::lang;
using namespace ppc::messages;

namespace ppc::comp::tree::ast {
    struct ast_ctx_t;
    using parser_func_t = bool (ast_ctx_t &ctx, size_t &res_i, data::map_t &out);
    using parser_t = parser_func_t*;

    class group_t {
    private:
        std::map<lang::namespace_name_t, std::string> named_parsers;
        std::set<std::string> unnamed_parsers;
        std::map<std::string, parser_t> parsers;
    public:
        group_t &replace(const std::string &name, parser_t parser);
        group_t &add(const std::string &name, parser_t parser);
        group_t &add(const std::string &name, const lang::namespace_name_t &identifier, parser_t parser);

        bool operator()(ast_ctx_t &ctx, size_t &i, data::map_t &out) const;
    };

    struct ast_ctx_t {
    private:
        std::unordered_map<std::string, group_t> groups;
    public:
        msg_stack_t &messages;
        std::vector<token_t> &tokens;
        std::set<loc_namespace_name_t> imports;
        loc_namespace_name_t nmsp;

        ast_ctx_t &operator=(const ast_ctx_t &other) = delete;

        template <class T>
        bool parse(const T &parser, size_t &i, data::map_t &out) {
            return parser(*this, i, out);
        }

        group_t &group(const std::string &name);

        template <class T>
        static data::map_t parse(const T &glob, msg_stack_t &messages, std::vector<token_t> &tokens) {
            ast_ctx_t ctx(messages, tokens);
            data::map_t res;
            size_t i = 0;

            if (!ctx.parse(glob, i, res)) throw message_t::error("Failed to compile.");
            return res;
        }

        ast_ctx_t(msg_stack_t &messages, std::vector<token_t> &tokens);
    };

    namespace conv {
        data::map_t identifier_to_map(const located_t<std::string> &loc);
        located_t<std::string> map_to_identifier(const data::map_t &map);

        data::string_t loc_to_map(const location_t &loc);
        location_t map_to_loc(const data::string_t &map);

        data::map_t nmsp_to_map(const loc_namespace_name_t &nmsp);
        loc_namespace_name_t map_to_nmsp(const data::map_t &map);
    }

    parser_func_t parse_glob, parse_nmsp, parse_identifier, parse_type, parse_exp, parse_stat_exp;
    parser_func_t parse_func, parse_field, parse_export, parse_struct;
    parser_func_t parse_if, parse_while, parse_return, parse_break, parse_continue, parse_stat_comp;
    parser_func_t parse_exp_var, parse_exp_str_lit, parse_exp_int_lit, parse_exp_float_lit;
}