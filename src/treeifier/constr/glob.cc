#include "treeifier/constr/glob.hh"
#include "treeifier/constr/helper.hh"
#include "treeifier/constr/nmsp.hh"

using namespace ppc::tree::constr;

bool ppc::tree::constr::glob_parser_t::operator()(ast_ctx_t &ctx, size_t &res_i, glob_t &out) const {
    parse_helper_t h(ctx, res_i);
    out = {};

    if (h.ended()) return h.submit(false);

    if (h.curr().is_identifier("namespace")) {
        h.advance("Expected a namespace");
        h.force_parse(nmsp_parser_t(), "Expected a namespace.", out.nmsp);

        if (!h.curr().is_operator(operator_t::SEMICOLON)) {
            ctx.messages.push(message_t::error("Expected a semicolon.", h.loc(1)));
        }

        if (!h.try_advance()) return h.submit(false);
    }

    while (h.curr().is_identifier("import")) {
        loc_namespace_name_t res;

        h.advance("Expected a namespace");
        h.force_parse(nmsp_parser_t(), "Expected a namespace.", res);

        if (!h.curr().is_operator(operator_t::SEMICOLON)) {
            ctx.messages.push(message_t::error("Expected a semicolon.", h.loc(1)));
        }

        out.imports.push_back(res);
        if (!h.try_advance()) return h.submit(false);
    }

    if (!h.ended()) h.err("Invalid token.");

    return h.submit(false);
}
