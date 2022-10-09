#include "compiler/treeifier/ast.hh"

namespace ppc::comp::tree::ast {
    std::unordered_map<std::string, group_parser_t> parsers;

    const parser_t &ast_ctx_t::parser_proxy_t::operator[](const std::string &name) const {
        auto it = parent->parsers.find(name);
        if (it == parent->parsers.end()) throw "The parser '" + name + "' doesn't exist.";
        return *it->second;
    }
    const group_parser_t &ast_ctx_t::group_proxy_t::operator[](const std::string &name) const {
        auto p = &parent->parser[name];
        if (parent->groups.find(p) == parent->groups.end()) throw "A parser '" + name + "' exists, but isn't a group.";
        return *(const group_parser_t*)p;
    }

    void ast_ctx_t::add_parser(const parser_t &parser) {
        if (parsers.find(parser.name()) != parsers.end()) throw "The parser '" + parser.name() + "' already exists.";
        parsers[parser.name()] = &parser;
    }
    void ast_ctx_t::add_parser(const group_parser_t &parser) {
        if (parsers.find(parser.name()) != parsers.end()) throw "The parser '" + parser.name() + "' already exists.";
        parsers[parser.name()] = &parser;
        groups.emplace(&parser);
    }

    bool ast_ctx_t::parse(msg_stack_t &messages, std::vector<token_t> &tokens, data::map_t &out) {
        ast_ctx_t ctx(messages, tokens);
        ctx.init();
        size_t i = 0;

        try {
            return glob_parser(ctx, i, out);
        }
        catch (const message_t &msg) {
            messages.push(msg);
            return false;
        }
    }
}

