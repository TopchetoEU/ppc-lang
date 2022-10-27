#include "compiler/treeifier/ast/helper.hh"

bool ast::parse_identifier(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (h.ended()) return false;

    if (h.curr().is_identifier()) {
        auto loc = h.loc();
        out["location"] = conv::loc_to_map(loc);
        out["content"] = h.curr().identifier();
        return h.submit();
    }
    else return false;
}
