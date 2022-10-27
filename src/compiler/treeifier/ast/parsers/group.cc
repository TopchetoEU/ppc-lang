#include "compiler/treeifier/ast.hh"
#include "compiler/treeifier/tokenizer.hh"
#include "compiler/treeifier/ast/helper.hh"
#include <algorithm>
#include <unordered_map>
#include <sstream>

using namespace ppc::comp::tree;
using namespace ppc::comp::tree::ast;
using namespace std::string_literals;
using namespace std;

static bool read_nmsp(ast_ctx_t &ctx, size_t &i, lang::loc_namespace_name_t &name) {
    tree_helper_t h(ctx, i);
    map_t res;
    if (!h.parse(parse_nmsp, res)) return false;
    name = conv::map_to_nmsp(res);
    return true;
}
template <class T>
static bool resolve_nmsp(ast_ctx_t &ctx, const lang::namespace_name_t &name, T begin, T end, lang::namespace_name_t &actual_name) {
    for (auto it = begin; it != end; it++) {
        const namespace_name_t &curr = it->first;
        if (curr == name) {
            actual_name = name;
            return true;
        }
    }
    for (const auto &import : ctx.imports) {
        auto new_name = name;
        new_name.insert(new_name.begin(), import.begin(), import.end());
        for (auto it = begin; it != end; it++) {
            const namespace_name_t &curr = it->first;
            if (curr == new_name) {
                actual_name = name;
                return true;
            }
        }
    }
    return false;
}

group_t &group_t::insert(const std::string &name, parser_t parser, const std::string &relative_to, bool after) {
    if (parsers.find(name) != parsers.end()) {
        throw "The parser '" + name + "' is already in the group.";
    }

    auto it = parsers.find(relative_to);
    if (it == parsers.end()) {
        throw "The parser '" + relative_to + "' isn't in the group.";
    }

    if (after) it++;

    parsers.insert(it, { name, parser });

    return *this;
}
group_t &group_t::replace(const std::string &name, parser_t parser) {
    auto it = parsers.find(name);

    if (parsers.find(name) == parsers.end()) {
        throw "The parser '" + name + "' isn't in the group.";
    }

    it->second = parser;

    return *this;
}
group_t &group_t::add_last(const std::string &name, parser_t parser) {
    if (parsers.find(name) != parsers.end()) {
        throw "The parser '" + name + "' is already in the group.";
    }

    parsers.emplace(name, parser);

    return *this;
}
group_t &group_t::add_named(const std::string &name, parser_t parser, const lang::namespace_name_t &identifier) {
    add_last(name, parser);
    return *this;
}

bool group_t::operator()(ast_ctx_t &ctx, size_t &i, data::map_t &out) const {
    tree_helper_t h(ctx, i);

    if (h.ended()) return false;

    loc_namespace_name_t name;
    if (read_nmsp(ctx, h.i, name)) {
        namespace_name_t actual;
        if (resolve_nmsp(ctx, name.strip_location(), named_parsers.begin(), named_parsers.end(), actual)) {
            auto parser = parsers.find(this->named_parsers.find(actual)->second);
            out["$_name"] = parser->first;
            if (parser->second(ctx, i, out)) return true;
            else throw message_t::error("Unexpected construct specifier.", h.res_loc());
        }
    }

    unordered_map<string, message_t> errors;

    for (auto parser : parsers) {
        out["$_name"] = parser.first;
        if ((*parser.second)(ctx, i, out)) return true;
    }

    stringstream m;

    return false;
}
