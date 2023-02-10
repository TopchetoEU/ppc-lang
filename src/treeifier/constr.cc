#include "treeifier/constr.hh"
#include "treeifier/parsers/helper.hh"
#include "lang/common.hh"

using namespace ppc::tree;

bool parse::parse_identifier(parse_ctx_t &ctx, size_t &res_i, located_t<std::string> &out) {
    helper_t h(ctx, res_i);

    if (h.ended()) return false;

    if (h.curr().is_identifier()) {
        out = located_t<std::string>(h.loc(), h.curr().identifier());
        return h.submit();
    }
    else return false;
}

bool parse::parse_nmsp(parse_ctx_t &ctx, size_t &res_i, loc_nmsp_t &out) {
    helper_t h(ctx, res_i);

    if (h.ended()) return false;

    out.clear();
    located_t<std::string> val;

    if (!h.parse(parse_identifier, val)) return false;
    else out.push_back(val);

    while (true) {
        if (h.ended()) break;
        if (!h.curr().is_operator(operator_t::DOUBLE_COLON)) break;
        h.advance("Expected an identifier.");
        h.force_parse(parse_identifier, "Expected an identifier.", val);
        out.push_back(val);
    }

    return h.submit(false);
}

bool parse::parse_nmsp_id(parse_ctx_t &ctx, size_t &res_i, std::set<nmsp_t> glob, nmsp_t nmsp) {
    helper_t h(ctx, res_i);
    loc_nmsp_t src;

    if (!h.parse(parse_nmsp, src)) return false;
    if (resolve_name(glob, src, nmsp)) return h.submit(false);
    else return false;
}
