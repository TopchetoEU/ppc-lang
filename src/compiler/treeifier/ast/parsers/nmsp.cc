#include "compiler/treeifier/ast/helper.hh"

bool ast::parse_nmsp(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (h.ended()) return false;

    auto &arr = (out["content"] = array_t()).array();

    if (!h.push_parse(parse_identifier, arr)) return false;

    while (true) {
        if (h.ended()) break;
        if (!h.curr().is_operator(operator_t::DOUBLE_COLON)) break;
        h.force_push_parse(parse_identifier, "Expected an identifier.", arr);
    }

    out["location"] = conv::loc_to_map(h.res_loc());
    return h.submit(false);
}
