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
    if (!h.parse("$_nmsp", res)) return false;
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


bool group_parser_t::parse(ast_ctx_t &ctx, size_t &i, data::map_t &out) const {
    tree_helper_t h(ctx, i);

    if (h.ended()) return false;

    loc_namespace_name_t name;
    if (read_nmsp(ctx, h.i, name)) {
        namespace_name_t actual;
        if (resolve_nmsp(ctx, name.strip_location(), named_parsers.begin(), named_parsers.end(), actual)) {
            auto &parser = *this->named_parsers.find(actual)->second;
            if (parser(ctx, i, out)) return true;
            else throw message_t::error("Unexpected construct specifier.", h.res_loc());
        }
    }

    unordered_map<string, message_t> errors;

    for (auto parser : parsers) {
        if ((*parser)(ctx, i, out)) return true;
    }

    stringstream m;

    return false;
}

group_parser_t &group_parser_t::add(const parser_t &parser) {
    parsers.push_back(&parser);
    return *this;
}
group_parser_t &group_parser_t::add(const parser_t &parser, const lang::namespace_name_t &name) {
    if (name.empty()) throw "Name can't be empty."s;
    if (std::find(parsers.begin(), parsers.end(), &parser) != parsers.end()) {
        throw "Parser '" + name.to_string() + "' already in group.";
    }

    named_parsers[name] = &parser;

    return *this;
}
