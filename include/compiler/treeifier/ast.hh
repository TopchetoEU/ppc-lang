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

    using parser_factory_t = parser_t *(*)();
    using group_parser_factory_t = group_parser_t *(*)();

    extern parser_factory_t glob_parser;
    extern parser_factory_t identifier_parser;
    extern parser_factory_t nmsp_parser;
    extern parser_factory_t type_parser;
    extern group_parser_factory_t def_parser;

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
            const group_parser_t &operator[](const std::string &name) const;
            group_proxy_t(ast_ctx_t *parent): parent(parent) { }
        };

        std::unordered_map<std::string, const parser_t*> parsers;
        std::set<const parser_t*> groups;

        void add_parser(const parser_t *parser);
        void add_parser(const group_parser_t *parser);
    public:
        msg_stack_t &messages;
        std::vector<token_t> &tokens;
        std::set<loc_namespace_name_t> imports;
        loc_namespace_name_t nmsp;

        void add_parser(parser_factory_t factory) { add_parser(factory()); }
        void add_parser(group_parser_factory_t factory) { add_parser(factory()); }

        ast_ctx_t &operator=(const ast_ctx_t &other) = delete;

        const parser_proxy_t parser;
        const group_proxy_t group;

        ast_ctx_t &init() {
            add_parser(glob_parser);
            add_parser(identifier_parser);
            add_parser(nmsp_parser);
            add_parser(def_parser);
            add_parser(type_parser);
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
        std::vector<std::pair<lang::namespace_name_t, parser_t*>> named_parsers;
        std::vector<parser_t*> parsers;
    public:
        group_parser_t &add(parser_t &parser);
        group_parser_t &add(parser_t &parser, const lang::namespace_name_t &name);

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