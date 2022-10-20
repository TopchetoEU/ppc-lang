#include "compiler/treeifier/ast/helper.hh"

class type_parser_t : public parser_t {
    bool parse(ast_ctx_t &ctx, size_t &res_i, map_t &out) const {
        tree_helper_t h(ctx, res_i);

        if (h.ended()) return false;

        auto &nmsp = (out["namespace"] = map_t()).map();
        nmsp["$_name"] = "$_nmsp";
        auto &nmsp_content = (out["namespace"].map()["content"] = array_t()).array();
        size_t ptr_n = 0;

        if (!h.push_parse("$_identifier", nmsp_content)) return false;

        while (true) {
            if (h.ended()) break;
            if (!h.curr().is_operator(operator_t::DOUBLE_COLON)) break;
            h.advance("Expected an identifier.");
            h.force_push_parse("$_identifier", "Expected an identifier.", nmsp_content);
        }

        while (!h.ended() && h.curr().is_operator(operator_t::MULTIPLY)) {
            ptr_n++;
            if (!h.try_advance()) break;
        }

        out["location"] = conv::loc_to_map(h.res_loc());
        out["name"] = nmsp_content[nmsp_content.size() - 1];
        out["ptr_n"] = (float)ptr_n;
        nmsp_content.pop_back();

        if (nmsp_content.size() == 0) {
            auto loc = h.res_loc();
            loc.length = 1;
            nmsp["location"] = conv::loc_to_map(loc);
        }
        else {
            auto loc_1 = conv::map_to_loc(nmsp_content[0].map()["location"].string());
            auto loc_2 = conv::map_to_loc(nmsp_content[nmsp_content.size() - 1].map()["location"].string());
            auto loc = loc_1.intersect(loc_2);
            nmsp["location"] = conv::loc_to_map(loc);
        }

        return h.submit(false);
    }

    public: type_parser_t(): parser_t("$_type") { }
};

const parser_adder_t ppc::comp::tree::ast::type_adder = [](ast_ctx_t &ctx) { ctx.add_parser(new type_parser_t()); };
