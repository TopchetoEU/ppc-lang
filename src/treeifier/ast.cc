#include "treeifier/constr.hh"
#include <iostream>

using namespace ppc;
using namespace ppc::lang;
using namespace ppc::tree;

// ppc::tree::constr::inspoint_t &parse_ctx_t::group(const std::string &name) {
//     if (groups.find(name) == groups.end()) return groups[name] = {};
//     else return groups[name];
// }

parse_ctx_t::parse_ctx_t(msg_stack_t &messages, std::vector<token_t> &tokens): messages(messages), tokens(tokens) {
    // group("$_exp_val")
    //     .add("$_var", parse_exp_var)
    //     .add("$_int", parse_exp_int_lit)
    //     .add("$_string", parse_exp_str_lit);
    // // .add_last("$_float", parse_exp_float_lit)
    // group("$_stat")
    //     .add("$_while", { "while" }, parse_while)
    //     .add("$_if", { "if" }, parse_if)
    //     .add("$_return", { "return" }, parse_return)
    //     .add("$_break", { "break" }, parse_break)
    //     .add("$_continue", { "continue" }, parse_continue)
    //     .add("$_comp", parse_stat_comp)
    //     .add("$_exp", parse_stat_exp);
    // group("$_def")
    //     .add("$_func", parse_func)
    //     .add("$_struct", { "struct" }, parse_struct)
    //     .add("$_field", parse_field);
    // group("$_struct_def")
    //     .add("$_func", parse_func)
    //     .add("$_field", parse_field);
}
