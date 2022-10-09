#include "compiler/treeifier/ast/helper.hh"

class identifier_parser_t : public parser_t {
    bool parse(ast_ctx_t &ctx, size_t &res_i, map_t &out) const {
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

    public: identifier_parser_t(): parser_t("$_identifier") { }
};

const parser_t &ppc::comp::tree::ast::identifier_parser = identifier_parser_t();
