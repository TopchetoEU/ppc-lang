#include "lang/module.hh"

using namespace std::string_literals;
using namespace ppc::lang;

definition_t::definition_t(function_t val) {
    this->kind = FUNCTION;
    this->val.func = new auto(val);
}
definition_t::definition_t(struct_t val) {
    this->kind = STRUCT;
    this->val.str = new auto(val);
}
definition_t::definition_t(field_t val) {
    this->kind = FIELD;
    this->val.field = new auto(val);
}
definition_t::definition_t(const definition_t &val) {
    this->kind = val.kind;
    switch (val.kind) {
        case STRUCT:
            this->val.str = new auto(*val.val.str);
            break;
        case FUNCTION:
            this->val.func = new auto(*val.val.func);
            break;
        case FIELD:
            this->val.field = new auto(*val.val.field);
            break;
    }
}

definition_t::~definition_t() {
    switch (this->kind) {
        case FUNCTION:
            delete this->val.func;
            break;
        case FIELD:
            delete this->val.field;
            break;
        case STRUCT:
            delete this->val.str;
            break;
    }
}


function_t &definition_t::get_func() {
    if (!this->is_func()) throw "Definition is not a function."s;
    return *this->val.func;
}
struct_t &definition_t::get_struct() {
    if (!this->is_struct()) throw "Definition is not a struct."s;
    return *this->val.str;
}
field_t &definition_t::get_field() {
    if (!this->is_field()) throw "Definition is not a field."s;
    return *this->val.field;
}


statement_t statement_t::call(const nmsp_t &func) {
    return { CALL, { .call = new auto(func) } };
}
statement_t statement_t::stack(int64_t stack) {
    return { STACK, { .stack = stack } };
}
statement_t statement_t::ret() {
    return { RETURN };
}
