#pragma once

#include <string>
#include <set>
#include <unordered_map>
#include <memory>
#include "compiler/treeifier/tokenizer.hh"
#include "utils/data.hh"
#include "lang/common.hh"

using namespace std::string_literals;
using namespace ppc;
using namespace ppc::messages;

namespace ppc::comp::tree::ast {
    class parser_t;
    class group_parser_t;

    struct ast_ctx_t {
    private:
        using named_parser_t = std::pair<std::string, parser_t*>;

        struct parser_proxy_t {
        private:
            ast_ctx_t &parent;
        public:
            parser_t &operator[](const std::string &name) const;
            parser_proxy_t(ast_ctx_t &parent): parent(parent) { }
        };

        struct group_proxy_t {
        private:
            ast_ctx_t &parent;
        public:
            group_parser_t &operator[](const std::string &name) const;
            group_proxy_t(ast_ctx_t &parent): parent(parent) { }
        };

        std::unordered_map<std::string, parser_t*> parsers;
        std::set<parser_t*> groups;

    public:
        msg_stack_t &messages;
        std::vector<tok::token_t> &tokens;

        void add_parser(std::string name, parser_t &parser);
        void add_parser(std::string name, group_parser_t &parser);

        const parser_proxy_t parser;
        const group_proxy_t group;

        ast_ctx_t(msg_stack_t &messages, std::vector<tok::token_t> tokens):
            messages(messages),
            tokens(tokens),
            parser(*this),
            group(*this) { }
    };

    class parser_t {
    private:
        std::string _name;
    public:
        const std::string &name() { return _name; }
        virtual bool parse(ast_ctx_t &ctx, size_t &res_i, data::map_t &out) const = 0;
        bool operator()(ast_ctx_t &ctx, size_t &i, data::map_t &out) const {
            return parse(ctx, i, out);
        }
    };

    class group_parser_t : public parser_t {
    private:
        std::vector<std::pair<lang::namespace_name_t, parser_t*>> named_parsers;
        std::vector<parser_t*> parsers;
    public:
        group_parser_t &add(parser_t &parser);
        group_parser_t &add(parser_t &parser, const lang::namespace_name_t &name);

        bool parse(ast_ctx_t &ctx, size_t &i, data::map_t &out) const;
    };

    extern const parser_t &glob_parser;

    const group_parser_t &get_group(std::string name);
}