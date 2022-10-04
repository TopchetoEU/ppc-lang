#include "compiler/treeifier/ast.hh"
#include "compiler/treeifier/tokenizer.hh"
#include "compiler/treeifier/ast/helper.hh"
#include <algorithm>

using namespace ppc::comp::tree;
using namespace ppc::comp::tree::ast;
using namespace std::string_literals;

static bool read_nmsp(ast_ctx_t &ctx, size_t &i, const lang::namespace_name_t &name) {
    tree_helper_t h(ctx, i);

    size_t equal_i = 0;

    while (true) {
        if (h.ended()) break;
        if (equal_i >= name.size()) return false;
        auto &curr = h.curr();
        if (!curr.is_identifier()) return false;

        if (name[equal_i] != curr.identifier()) return false;

        if (h.try_advance() && h.curr().is_operator(operator_t::DOUBLE_COLON)) {
            equal_i++;
        }
        else break;
    }

    return equal_i != name.size();
}

bool group_parser_t::parse(ast_ctx_t &ctx, size_t &i, data::map_t &out) const {
    tree_helper_t h(ctx, i);

    for (auto &pair : named_parsers) {
        if (!read_nmsp(ctx, i, pair.first)) continue;
        auto &parser = *pair.second;
        return h.parse(parser, out);
    }
    for (auto parser : parsers) {
        try {
            return h.parse(*parser, out);
        }
        catch (std::string) {
            return false;
        }
    }
}


group_parser_t &group_parser_t::add(parser_t &parser) {
    parsers.push_back(&parser);
    return *this;
}
group_parser_t &group_parser_t::add(parser_t &parser, const lang::namespace_name_t &name) {
    if (name.empty()) throw "Name can't be empty."s;
    if (std::find(parsers.begin(), parsers.end(), &parser) != parsers.end()) {
        throw "Parser '" + name.to_string() + "' already in group.";
    }

    named_parsers.push_back({ name, &parser });

    return *this;
}
