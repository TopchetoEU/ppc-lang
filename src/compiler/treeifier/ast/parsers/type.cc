#include "compiler/treeifier/ast/helper.hh"

bool ast::parse_type(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (h.ended()) return false;

    auto &nmsp = out["namespace"].map({});
    size_t ptr_n = 0;

    if (!h.parse(parse_nmsp, nmsp)) return false;

    while (!h.ended() && h.curr().is_operator(operator_t::MULTIPLY)) {
        ptr_n++;
        if (!h.try_advance()) break;
    }

    auto &nmsp_arr = nmsp["content"].array();

    h.i--;
    out["location"] = conv::loc_to_map(h.res_loc());
    h.i++;
    out["name"] = nmsp_arr.back();
    out["ptr_n"] = (float)ptr_n;
    nmsp_arr.pop_back();
    if (nmsp_arr.empty()) out["namespace"] = null;
    else {
        auto loc_1 = conv::map_to_loc(nmsp_arr.front().map()["location"].string());
        auto loc_2 = conv::map_to_loc(nmsp_arr.back().map()["location"].string());
        auto loc = loc_1.intersect(loc_2);
        nmsp["location"] = conv::loc_to_map(loc);
    }

    return h.submit(false);
}
