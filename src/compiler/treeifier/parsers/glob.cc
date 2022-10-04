#include "compiler/treeifier/ast.hh"

namespace ppc::comp::tree::ast {
    class glob_parser_t : public parser_t {
        bool parse(ast_ctx_t &ctx, size_t &res_i, data::map_t &out) const {
            return false;
        }
    };

    const parser_t &glob_parser = glob_parser_t();
}
