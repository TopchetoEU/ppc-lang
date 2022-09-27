#include "compiler/treeifier/ast.hh"

namespace ppc::comp::tree::ast {
    class glob_parser_t : public constr_parser_t {
        bool parse(messages::msg_stack_t &messages, vec_slice_t<tok::token_t> &tokens, data::map_t &out) {
            
        }
    };

    const constr_parser_t &glob_parser = glob_parser_t();
}
