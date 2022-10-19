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
using namespace ppc::lang;
using namespace ppc::messages;

namespace ppc::comp::tree::ast {
    class parser_t;
    class group_parser_t;
    struct ast_ctx_t;

    using parser_adder_t = void (*)(ast_ctx_t &ctx);

    extern const parser_adder_t glob_adder;
    extern const parser_adder_t identifier_adder;
    extern const parser_adder_t nmsp_adder;
    extern const parser_adder_t type_adder;
    extern const parser_adder_t exp_adder;
    extern const parser_adder_t field_adder;
    extern const parser_adder_t var_adder;

    struct ast_ctx_t {
    private:
        struct parser_proxy_t {
        private:
            ast_ctx_t *parent;
        public:
            const parser_t &operator[](const std::string &name) const;
            parser_proxy_t(ast_ctx_t *parent): parent(parent) { }
        };

        struct group_proxy_t {
        private:
            ast_ctx_t *parent;
        public:
            group_parser_t &operator[](const std::string &name) const;
            group_proxy_t(ast_ctx_t *parent): parent(parent) { }
        };

        std::unordered_map<std::string, const parser_t*> parsers;
        std::set<group_parser_t*> groups;
    public:
        msg_stack_t &messages;
        std::vector<token_t> &tokens;
        std::set<loc_namespace_name_t> imports;
        loc_namespace_name_t nmsp;

        void add_parser(const parser_t *parser);
        void add_parser(const parser_t *parser, const std::string &group);
        void add_group(const std::string &name);

        void add_parser(parser_adder_t factory) { factory(*this); }

        ast_ctx_t &operator=(const ast_ctx_t &other) = delete;

        const parser_proxy_t parser;
        const group_proxy_t group;

        ast_ctx_t &init() {
            add_parser(identifier_adder);
            add_parser(nmsp_adder);
            add_parser(glob_adder);
            add_parser(type_adder);
            add_parser(exp_adder);
            add_parser(var_adder);
            add_parser(field_adder);

            return *this;
        }

        bool parse(std::string parser, size_t &pi, data::map_t &out);
        static data::map_t parse(msg_stack_t &messages, std::vector<token_t> &tokens);

        ~ast_ctx_t();
        ast_ctx_t(msg_stack_t &messages, std::vector<token_t> &tokens):
            messages(messages),
            tokens(tokens),
            parser(this),
            group(this) { }
    };

    class parser_t {
    private:
        std::string _name;
    protected:
        virtual bool parse(ast_ctx_t &ctx, size_t &res_i, data::map_t &out) const = 0;
    public:
        const std::string &name() const { return _name; }
        bool operator()(ast_ctx_t &ctx, size_t &i, data::map_t &out) const {
            data::map_t res;
            out["$_name"] = _name;
            return parse(ctx, i, out);
        }

        virtual ~parser_t() = default;
        parser_t(const std::string &name): _name(name) { }
    };

    class group_parser_t : public parser_t {
    private:
        std::vector<std::pair<lang::namespace_name_t, const parser_t*>> named_parsers;
        std::vector<const parser_t*> parsers;
    public:
        group_parser_t &add(const parser_t &parser);
        group_parser_t &add(const parser_t &parser, const lang::namespace_name_t &name);

        bool parse(ast_ctx_t &ctx, size_t &i, data::map_t &out) const;

        group_parser_t(const std::string &name): parser_t(name) { }
    };

    namespace conv {
        data::map_t identifier_to_map(const located_t<std::string> &loc);
        located_t<std::string> map_to_identifier(const data::map_t &map);

        data::map_t loc_to_map(const location_t &loc);
        location_t map_to_loc(const data::map_t &map);

        data::map_t nmsp_to_map(const loc_namespace_name_t &nmsp);
        loc_namespace_name_t map_to_nmsp(const data::map_t &map);
    }
}