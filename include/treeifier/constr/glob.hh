#include "treeifier/constr.hh"

namespace ppc::tree::constr {
    class glob_parser_t: public parser_t<glob_t> {
    public:
        bool operator()(ast_ctx_t &ctx, size_t &res_i, glob_t &out) const override;
        bool simplify(ast_ctx_t &ctx, glob_t &glob, glob_t &val) const override { return false; }
    };
}