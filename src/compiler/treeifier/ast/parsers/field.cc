#include "compiler/treeifier/ast/helper.hh"

bool ast::parse_field(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (h.ended()) return false;

    if (!h.parse(parse_identifier, out["name"].map({}))) return false;

    bool type, defval;

    h.throw_ended("Expected a colon or an equals sign.");

    if (h.curr().is_operator(operator_t::COLON)) {
        h.advance();
        h.force_parse(parse_type, "Expected a type.", out["type"].map({}));
        type = true;
    }
    if (h.curr().is_operator(operator_t::ASSIGN)) {
        h.advance();
        h.force_parse(parse_exp, "Expected an expression.", out["value"].map({}));
        type = true;
    }

    if (h.curr().is_operator(operator_t::SEMICOLON)) {
        if (type || defval) return h.submit();
        else return h.err("A type or a default value must be specified ");
    }
    else if (type || defval) {
        ctx.messages.push(message_t::error("Expected a semicolon.", h.loc(1)));
        return h.submit(false);
    }
    else return false;

    return h.submit(true);
}
