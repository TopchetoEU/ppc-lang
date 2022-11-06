#include "compiler/treeifier/ast/helper.hh"

bool ast::parse_if(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    h.throw_ended("Expected open parens after if keyword.");
    if (!h.curr("Expected open parens after if keyword.").is_operator(operator_t::PAREN_OPEN)) {
        throw message_t::error("Expected open parens after if keyword.", h.loc(1));
    }

    h.advance("Expected an expression.");
    h.force_parse(parse_exp, "Expected an expression.", out["condition"].map({}));

    if (!h.curr("Expected closed parens.").is_operator(operator_t::PAREN_CLOSE)) {
        throw message_t::error("Expected closed parens.", h.loc(1));
    }

    h.advance("Expected a statement.");
    h.force_parse(ctx.group("$_stat"), "Expected a statement.", out["if"].map({}));

    if (h.ended() || !h.curr().is_identifier("else")) return h.submit(false);

    h.advance("Expected a statement.");
    h.force_parse(ctx.group("$_stat"), "Expected a statement.", out["else"].map({}));

    return h.submit(false);
}

bool ast::parse_while(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    h.throw_ended("Expected open parens after while keyword.");
    if (!h.curr("Expected open parens after while keyword.").is_operator(operator_t::PAREN_OPEN)) {
        throw message_t::error("Expected open parens after while keyword.", h.loc(1));
    }

    h.advance("Expected an expression.");
    h.force_parse(parse_exp, "Expected an expression.", out["condition"].map({}));

    if (!h.curr("Expected closed parens.").is_operator(operator_t::PAREN_CLOSE)) {
        throw message_t::error("Expected closed parens.", h.loc(1));
    }

    h.advance("Expected a statement.");
    h.force_parse(ctx.group("$_stat"), "Expected a statement.", out["while"].map({}));

    return h.submit(false);
}

bool ast::parse_return(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    h.throw_ended("Expected an expression.");
    h.force_parse(parse_exp, "Expected an expression.", out["condition"].map({}));

    if (!h.curr("Expected a semicolon.").is_operator(operator_t::SEMICOLON)) {
        throw message_t::error("Expected a semicolon.", h.loc(1));
    }

    return h.submit(true);
}

bool ast::parse_break(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (!h.curr("Expected a semicolon.").is_operator(operator_t::SEMICOLON)) {
        throw message_t::error("Expected a semicolon.", h.loc(1));
    }

    return h.submit(true);
}

bool ast::parse_continue(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (!h.curr("Expected a semicolon.").is_operator(operator_t::SEMICOLON)) {
        throw message_t::error("Expected a semicolon.", h.loc(1));
    }

    return h.submit(true);
}

bool ast::parse_stat_comp(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (h.ended()) return false;
    if (!h.curr().is_operator(operator_t::BRACE_OPEN)) return false;
    h.advance("Expected a statement or a closing brace.");

    auto &content = out["content"].array({});

    while (!h.curr().is_operator(operator_t::BRACE_CLOSE)) {
        h.throw_ended("Expected a statement or a closing brace.");
        h.push_parse(ctx.group("$_stat"), content);
    }

    return h.submit(true);
}