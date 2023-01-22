#include "treeifier/constr/helper.hh"
#include "treeifier/constr/nmsp.hh"

using namespace ppc::tree::constr;

bool nmsp_parser_t::operator()(ast_ctx_t &ctx, size_t &res_i, loc_namespace_name_t &out) const {
    parse_helper_t h(ctx, res_i);

    if (h.ended()) return false;

    out.clear();
    located_t<std::string> val;

    if (!h.parse(identifier_parser_t(), val)) return false;
    else out.push_back(val);

    while (true) {
        if (h.ended()) break;
        if (!h.curr().is_operator(operator_t::DOUBLE_COLON)) break;
        h.advance("Expected an identifier.");
        h.force_parse(identifier_parser_t(), "Expected an identifier.", val);
        out.push_back(val);
    }

    return h.submit(false);
}
