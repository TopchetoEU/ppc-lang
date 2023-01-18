#include "compiler/treeifier/ast/helper.hh"

static bool parse_arg(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (h.ended()) return false;

    h.parse(parse_export, out);

    if (!h.parse(parse_identifier, out["name"].map({}))) return false;

    bool type = false, defval = false;

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

    if (!type && !defval) {
        ctx.messages.push(message_t::error("Expected a type or a default value.", h.loc(1)));
    }

    return h.submit(false);
}

bool ast::parse_func(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (h.ended()) return false;

    h.parse(parse_export, out);

    if (!h.parse(parse_identifier, out["name"].map({}))) return false;
    if (h.ended()) return false;
    if (!h.curr().is_operator(operator_t::PAREN_OPEN)) return false;
    h.advance("Expected a closing paren or a parameter.");

    auto &params = out["params"].array({});
    auto &content = out["content"].array({});

    while (true) {
        if (h.curr().is_operator(operator_t::PAREN_CLOSE)) {
            h.advance("Expected a function body.");
            break;
        }
        h.force_push_parse(parse_arg, "Expected a parameter.", params);
        if (h.curr().is_operator(operator_t::COMMA)) {
            h.advance("Expected a parameter.");
        }
    }

    if (h.curr().is_operator(operator_t::COLON)) {
        h.advance("Expected a type.");
        h.force_parse(parse_type, "Expected a type", out["type"].map({}));
    }

    if (h.curr().is_operator(operator_t::SEMICOLON)) return h.submit(true);
    else if (h.curr().is_operator(operator_t::LAMBDA)) {
        h.advance("Expected an expression.");
        map_t exp;
        h.force_parse(parse_exp, "Expected an expression.", exp);
        content.push_back({
            { "$_name", "$_return" },
            { "content", exp },
        });
        return h.submit(false);
    }
    else if (h.curr().is_operator(operator_t::BRACE_OPEN)) {
        h.advance("Expected a statement.");
        while (true) {
            if (h.curr().is_operator(operator_t::BRACE_CLOSE)) {
                return h.submit(true);
            }

            h.force_push_parse(ctx.group("$_stat"), "Expected an expression.", content);
        }
    }
    else {
        ctx.messages.push(message_t::error("Expected a semicolon, brace open or a lambda operator.", h.loc(1)));
        return h.submit(false);
    }

    return h.submit(true);
}
