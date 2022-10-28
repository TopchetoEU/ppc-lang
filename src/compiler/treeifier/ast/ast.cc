#include "compiler/treeifier/ast.hh"

using namespace ppc;
using namespace ppc::data;
using namespace ppc::lang;
using namespace ppc::comp::tree::ast;

group_t &ast_ctx_t::group(const std::string &name) {
    if (groups.find(name) == groups.end()) return groups[name] = { };
    else return groups[name];
}

ast_ctx_t::ast_ctx_t(msg_stack_t &messages, std::vector<token_t> &tokens): messages(messages), tokens(tokens) {
    group("$_exp_val")
        .add_last("$_var", parse_exp_var)
        .add_last("$_int", parse_exp_int_lit)
        .add_last("$_string", parse_exp_str_lit);
        // .add_last("$_float", parse_exp_float_lit)
    group("$_stat")
        .add_named("$_while", parse_while, { "while" })
        .add_named("$_if", parse_if, { "if" })
        .add_named("$_return", parse_return, { "return" })
        .add_named("$_break", parse_break, { "break" })
        .add_named("$_continue", parse_continue, { "continue" })
        .add_last("$_comp", parse_stat_comp)
        .add_last("$_exp", parse_stat_exp);
    group("$_def")
        .add_last("$_func", parse_func)
        .add_named("$_export", parse_export, { "export" })
        .add_last("$_field", parse_field);
}