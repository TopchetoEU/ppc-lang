#include "compiler/treeifier/ast/helper.hh"

class field_parser_t : public parser_t {
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
            h.i++;
            h.err("Default values are not yet supported.", 1);
            h.advance();
            h.force_parse("$_exp", "Expected an expression.", out["value"].map({}));
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

    public: field_parser_t(): parser_t("$_field") { }
};

const parser_adder_t ppc::comp::tree::ast::field_adder = [](ast_ctx_t &ctx) { ctx.add_parser(new field_parser_t(), "$_def"); };
