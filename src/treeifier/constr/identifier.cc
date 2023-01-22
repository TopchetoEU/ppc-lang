#include "treeifier/constr/helper.hh"
#include "treeifier/constr/identifier.hh"

using namespace ppc::tree::constr;

bool identifier_parser_t::operator()(ast_ctx_t& ctx, size_t& res_i, located_t<std::string>& out) const {
    parse_helper_t h(ctx, res_i);

    if (h.ended()) return false;

    if (h.curr().is_identifier()) {
        out = located_t<std::string>(h.loc(), h.curr().identifier());
        return h.submit();
    }
    else return false;
}
