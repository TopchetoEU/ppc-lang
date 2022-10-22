#include "compiler/treeifier/ast/helper.hh"

class arg_parser_t : public parser_t {
    bool parse(ast_ctx_t &ctx, size_t &res_i, map_t &out) const {
        tree_helper_t h(ctx, res_i);

        if (h.ended()) return false;

        if (!h.parse("$_identifier", out["name"].map({}))) return false;

        bool type, defval;

        h.throw_ended("Expected a colon or an equals sign.");

        if (h.curr().is_operator(operator_t::COLON)) {
            h.advance();
            h.force_parse("$_type", "Expected a type.", out["type"].map({}));
            type = true;
        }
        if (h.curr().is_operator(operator_t::ASSIGN)) {
            h.advance();
            h.force_parse("$_exp", "Expected an expression.", out["value"].map({}));
            type = true;
        }

        if (!type && !defval) {
            ctx.messages.push(message_t::error("Expected a type or a default value.", h.loc(1)));
        }

        return h.submit(false);
    }

    public: arg_parser_t(): parser_t("$_func_arg") {}
};

class func_parser_t : public parser_t {
    bool parse(ast_ctx_t &ctx, size_t &res_i, map_t &out) const {
        tree_helper_t h(ctx, res_i);

        if (h.ended()) return false;

        if (!h.parse("$_identifier", out["name"].map({}))) return false;
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
            h.force_push_parse("$_func_arg", "Expected a parameter.", params);
            if (h.curr().is_operator(operator_t::COMMA)) {
                h.advance("Expected a parameter.");
            }
        }

        if (h.curr().is_operator(operator_t::COLON)) {
            h.advance("Expected a type.");
            h.force_parse("$_type", "Expected a type", out["type"].map({}));
        }

        if (h.curr().is_operator(operator_t::SEMICOLON)) return h.submit(true);
        else if (h.curr().is_operator(operator_t::LAMBDA)) {
            h.advance("Expected an expression.");
            map_t exp;
            h.force_parse("$_exp", "Expected an expression.", exp);
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

                h.force_push_parse("$_stat", "Expected an expression.", content);
            }
        }
        else {
            ctx.messages.push(message_t::error("Expected a semicolon, brace open or a lambda operator.", h.loc(1)));
            return h.submit(false);
        }

        return h.submit(true);
    }

    public: func_parser_t(): parser_t("$_func") { }
};

const parser_adder_t ppc::comp::tree::ast::func_adder = [](ast_ctx_t &ctx) {
    ctx.add_parser(new func_parser_t(), "$_def");
    ctx.add_parser(new arg_parser_t());
};
