#include "treeifier/constr/identifier.hh"
#include "treeifier/constr.hh"

namespace ppc::tree::constr {
    struct nmsp_parser_t: public parser_t<loc_namespace_name_t> {
        bool operator()(ast_ctx_t &ctx, size_t &res_i, loc_namespace_name_t &out) const override;
        bool simplify(ast_ctx_t &ctx, glob_t &glob, loc_namespace_name_t &val) const override { return false; }
    };
}