#include "compiler/treeifier/ast/helper.hh"

bool ast::parse_struct(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (h.ended()) return false;

    if (!h.parse(parse_identifier, out["name"].map({}))) return false;

    auto &content = out["content"].array({});

    if (h.ended()) return false;
    if (h.curr().is_operator(operator_t::SEMICOLON)) return h.submit(true);
    if (!h.curr().is_operator(operator_t::BRACE_OPEN)) return false;
    h.advance("Expected a definition, a closing brace, or a semicolon.");


    while (!h.curr().is_operator(operator_t::BRACE_CLOSE)) {
        h.throw_ended("Expected a definition or a closing brace.");
        h.push_parse(ctx.group("$_struct_def"), content);
    }

    return h.submit(true);
}
