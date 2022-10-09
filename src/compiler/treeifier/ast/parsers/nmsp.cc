#include "compiler/treeifier/ast/helper.hh"

class nmsp_parser_t : public parser_t {
    bool parse(ast_ctx_t &ctx, size_t &res_i, map_t &out) const {
        tree_helper_t h(ctx, res_i);

        if (h.ended()) return false;

        auto &arr = (out["content"] = array_t()).array();

        if (!h.push_parse("$_identifier", arr)) return false;

        while (true) {
            if (h.ended()) break;
            if (!h.curr().is_operator(operator_t::DOUBLE_COLON)) break;
            h.force_push_parse("$_identifier", "Expected an identifier.", arr);
        }

        out["location"] = conv::loc_to_map(h.res_loc());
        return h.submit(false);
    }

    public: nmsp_parser_t(): parser_t("$_nmsp") { }
};

parser_factory_t ppc::comp::tree::ast::nmsp_parser = []() { return (parser_t*)new nmsp_parser_t(); };
