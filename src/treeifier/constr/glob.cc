#include "treeifier/constr.hh"
#include "treeifier/constr/helper.hh"

using namespace ppc::tree::constr;

bool ppc::tree::constr::glob_parser_t::operator()(ast_ctx_t &ctx, glob_t &out) const {
    size_t res_i = 0;
    helper_t h(ctx, res_i);
    out = {};

    if (h.ended()) return h.submit(false);

    if (h.curr().is_identifier("namespace")) {
        h.advance("Expected a namespace");
        h.force_parse(parse_nmsp, "Expected a namespace.", out.nmsp);

        if (!h.curr().is_operator(operator_t::SEMICOLON)) {
            ctx.messages.push(message_t::error("Expected a semicolon.", h.loc(1)));
        }

        if (!h.try_advance()) return h.submit(false);
    }

    while (h.curr().is_identifier("import")) {
        loc_nmsp_t res;

        h.advance("Expected a namespace");
        h.force_parse(parse_nmsp, "Expected a namespace.", res);

        if (!h.curr().is_operator(operator_t::SEMICOLON)) {
            ctx.messages.push(message_t::error("Expected a semicolon.", h.loc(1)));
        }

        out.imports.push_back(res);
        if (!h.try_advance()) return h.submit(false);
    }

    if (!h.ended()) h.err("Invalid token.");

    return h.submit(false);
}
