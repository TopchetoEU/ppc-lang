#include "compiler/treeifier/ast.hh"
#include "compiler/treeifier/ast/helper.hh"

using namespace ppc::comp::tree::ast;

static bool nmsp_def(ast_ctx_t &ctx, size_t &res_i, data::map_t &res) {
    tree_helper_t h(ctx, res_i);
    if (h.ended()) return false;

    if (!h.curr().is_identifier("namespace")) return false;
    h.advance("Expected a namespace");
    h.force_parse(parse_nmsp, "Expected a namespace.", res);
    if (!h.curr().is_operator(operator_t::SEMICOLON)) {
        ctx.messages.push(message_t::error("Expected a semicolon.", h.loc(1)));
        return h.submit(false);
    }
    return h.submit(true);
}
static bool import(ast_ctx_t &ctx, size_t &res_i, data::map_t &res) {
    tree_helper_t h(ctx, res_i);
    if (h.ended()) return false;

    if (!h.curr().is_identifier("import")) return false;
    h.advance("Expected a namespace");
    h.force_parse(parse_nmsp, "Expected a namespace.", res);
    if (!h.curr().is_operator(operator_t::SEMICOLON)) {
        ctx.messages.push(message_t::error("Expected a semicolon.", h.loc(1)));
        return h.submit(false);
    }

    return h.submit(true);
}

bool ast::parse_glob(ast_ctx_t &ctx, size_t &res_i, data::map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (h.ended()) return true;
    if (h.parse(nmsp_def, out["namespace"].map({}))) {
        ctx.nmsp = conv::map_to_nmsp(out["namespace"].map());
    }
    else {
        out["namespace"] = data::null;
    }

    auto &imports = out["imports"].array({});
    auto &contents = out["content"].array({});

    while (true) {
        map_t map;
        if (!h.parse(import, map)) break;
        imports.push_back(map);
        auto nmsp = conv::map_to_nmsp(map);

        if (!ctx.imports.emplace(nmsp).second) h.err("The namespace '" + nmsp.to_string() + "' is already imported.");
    }

    while (true) {
        if (h.ended()) break;
        if (!h.push_parse(ctx.group("$_def"), contents)) {
            ctx.messages.push(message_t::error("Invalid token.", h.loc()));
            h.i++;
        }
    }

    if (!h.ended()) h.err("Invalid token.");

    return h.submit();
}
