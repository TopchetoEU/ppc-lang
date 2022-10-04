#include "compiler/treeifier/ast.hh"
#include <algorithm>

using namespace ppc::comp::tree::ast;
using namespace std::string_literals;

group_parser_t &group_parser_t::add(parser_t &parser) {
    parsers.push_back(&parser);
    return *this;
}
group_parser_t &group_parser_t::add(parser_t &parser, const lang::namespace_name_t &name) {
    if (name.is_empty()) throw "Name can't be empty."s;
    if (std::find(parsers.begin(), parsers.end(), &parser) != parsers.end()) {
        throw "Parser '" + name.to_string() + "' already in group.";
    }

    named_parsers.push_back({ name, &parser });

    return *this;
}
