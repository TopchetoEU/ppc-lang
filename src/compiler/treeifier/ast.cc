#include "compiler/treeifier/ast.hh"

namespace ppc::comp::tree::ast {
    std::unordered_map<std::string, group_parser_t> parsers;

    const parser_t &ast_ctx_t::parser_proxy_t::operator[](const std::string &name) const {
        auto it = parent->parsers.find(name);
        if (it == parent->parsers.end()) throw "The parser '" + name + "' doesn't exist.";
        return *it->second;
    }
    group_parser_t &ast_ctx_t::group_proxy_t::operator[](const std::string &name) const {
        auto p = (group_parser_t*)&parent->parser[name];
        if (parent->groups.find(p) == parent->groups.end()) throw "A parser '" + name + "' exists, but isn't a group.";
        return *p;
    }

    ast_ctx_t::~ast_ctx_t() {
        for (auto pair : parsers) {
            delete pair.second;
        }
    }

    void ast_ctx_t::add_parser(const parser_t *parser) {
        if (parsers.find(parser->name()) != parsers.end()) throw "The parser '" + parser->name() + "' already exists.";
        parsers[parser->name()] = parser;
    }
    void ast_ctx_t::add_parser(const parser_t *parser, const std::string &group) {
        add_parser(parser);
        this->group[group].add(*parser);
    }
    void ast_ctx_t::add_parser(const parser_t *parser, const std::string &group, const namespace_name_t &name) {
        add_parser(parser);
        this->group[group].add(*parser, name);
    }
    void ast_ctx_t::add_group(const std::string &name) {
        auto parser = new group_parser_t(name);
        if (parsers.find(parser->name()) != parsers.end()) throw "The parser '" + parser->name() + "' already exists.";
        parsers[parser->name()] = parser;
        groups.emplace(parser);
    }

    data::map_t ast_ctx_t::parse(msg_stack_t &messages, std::vector<token_t> &tokens) {
        ast_ctx_t ctx(messages, tokens);
        ctx.init();
        size_t i = 0;
        data::map_t res;

        if (!ctx.parse("$_glob", i, res)) throw message_t::error("Failed to compile.");
        return res;
    }
    bool ast_ctx_t::parse(std::string parser, size_t &pi, data::map_t &out) {
        return this->parser[parser] (*this, pi, out);
    }
}

