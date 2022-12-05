#include "lang/module.hh"
#include "compiler/treeifier/ast.hh"
#include "compiler/treeifier/tokenizer.hh"
#include "compiler/treeifier/ast/helper.hh"
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <string>

using namespace ppc::comp::tree;
using namespace ppc::comp::tree::ast;
using namespace std::string_literals;
using namespace std;

static bool read_nmsp(ast_ctx_t &ctx, size_t &i, lang::loc_namespace_name_t &name) {
    tree_helper_t h(ctx, i);
    map_t res;
    if (!h.parse(parse_nmsp, res)) return false;
    name = conv::map_to_nmsp(res);
    return h.submit(false);
}


group_t &group_t::insert(const std::string &name, parser_t parser, const std::string &relative_to, bool after) {
    if (parsers.find(name) != parsers.end()) {
        throw "The parser '" + name + "' is already in the group.";
    }

    auto it = unnamed_parsers.find(relative_to);
    if (it == unnamed_parsers.end()) {
        throw "The parser '" + relative_to + "' isn't in the group or isn't unnamed.";
    }

    if (after) it++;

    unnamed_parsers.insert(it, name);

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
    unnamed_parsers.emplace(name);

    return *this;
}
group_t &group_t::add_named(const std::string &name, parser_t parser, const lang::namespace_name_t &identifier) {
    if (parsers.find(name) != parsers.end()) {
        throw "The parser '" + name + "' is already in the group.";
    }

    parsers.emplace(name, parser);
    named_parsers.emplace(identifier, name);

    return *this;
}

bool group_t::operator()(ast_ctx_t &ctx, size_t &i, data::map_t &out) const {
    tree_helper_t h(ctx, i);

    if (h.ended()) return false;

    std::set<namespace_name_t> names;

    for (auto &import : ctx.imports) names.insert(import.strip_location());

    loc_namespace_name_t name;
    if (read_nmsp(ctx, h.i, name)) {
        namespace_name_t actual;

        if (resolve_name_map(
            named_parsers, names,
            name.strip_location(), actual
        )) {
            auto parser = parsers.find(this->named_parsers.find(actual)->second);
            out["$_name"] = parser->first;
            if (h.parse(parser->second, out)) return h.submit(false);
            else throw message_t::error("Unexpected construct specifier.", h.res_loc());
        }
    }

    for (auto name : unnamed_parsers) {
        out["$_name"] = name;
        if (parsers.at(name)(ctx, i, out)) return true;
    }

    stringstream m;

    return false;
}
