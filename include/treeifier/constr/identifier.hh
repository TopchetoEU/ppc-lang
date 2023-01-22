#include "treeifier/constr.hh"

namespace ppc::tree::constr {
    struct identifier_parser_t: public parser_t<located_t<std::string>> {
        bool operator()(ast_ctx_t &ctx, size_t &res_i, located_t<std::string> &out) const override;
        bool simplify(ast_ctx_t &ctx, glob_t &glob, located_t<std::string> &val) const override { return false; }
    };
}