#include "compiler/treeifier/ast.hh"

namespace ppc::comp::tree::ast {
    std::unordered_map<std::string, group_parser_t> parsers;

    parser_t &ast_ctx_t::parser_proxy_t::operator[](const std::string &name) const {
        auto it = parent.parsers.find(name);
        if (it == parent.parsers.end()) throw "The parser '" + name + "' doesn't exist.";
        return *it->second;
    }
    group_parser_t &ast_ctx_t::group_proxy_t::operator[](const std::string &name) const {
        parser_t *p = &parent.parser[name];
        if (parent.groups.find(p) == parent.groups.end()) throw "A parser '" + name + "' exists, but isn't a group.";
        return *(group_parser_t*)p;
    }

    void ast_ctx_t::add_parser(std::string name, parser_t &parser) {
        if (parsers.find(name) != parsers.end()) throw "The parser '" + name + "' already exists.";
        parsers[name] = &parser;
    }
    void ast_ctx_t::add_parser(std::string name, group_parser_t &parser) {
        if (parsers.find(name) != parsers.end()) throw "The parser '" + name + "' already exists.";
        parsers[name] = &parser;
        groups.emplace(&parser);
    }
}

