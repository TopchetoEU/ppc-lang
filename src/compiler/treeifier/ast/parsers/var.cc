#include "compiler/treeifier/ast/helper.hh"

class var_parser_t : public parser_t {
    bool parse(ast_ctx_t &ctx, size_t &res_i, map_t &out) const {
        tree_helper_t h(ctx, res_i);

        if (h.curr().is_identifier()) {
            out["content"] = h.curr().identifier();
            out["location"] = conv::loc_to_map(h.loc());
            return h.submit(true);
        }

        return false;
    }

    public: var_parser_t(): parser_t("$_var") { }
};

const parser_adder_t ppc::comp::tree::ast::var_adder = [](ast_ctx_t &ctx) { ctx.add_parser(new var_parser_t(), "$_exp_val"); };
