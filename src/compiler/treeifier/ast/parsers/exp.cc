#include "compiler/treeifier/ast/helper.hh"
#include <map>
#include <algorithm>

enum precedence_t {
    NONE,
    POSTFIX,
    PREFIX,
    MULT,
    ADD,
    SHIFT,
    COMP,
    EQU,
    BIN_AND,
    BIN_XOR,
    BIN_OR,
    BOOL_AND,
    BOOL_OR,
    TERNARY,
    ASSIGN,
    PAREN,
    CALL_START,
};

struct op_data_t {
    precedence_t precedence;
    size_t op_n;
    std::string name;
    bool assoc;
};

op_data_t sizeof_data { precedence_t::PREFIX, 1, "sizeof", true };

std::map<operator_t, op_data_t> pre_ops {
    { operator_t::INCREASE, { precedence_t::PREFIX, 1, "inc_pre" } },
    { operator_t::DECREASE, { precedence_t::PREFIX, 1, "dec_pre" } },
    { operator_t::ADD, { precedence_t::PREFIX, 1, "positive" } },
    { operator_t::SUBTRACT, { precedence_t::PREFIX, 1, "negative" } },
    { operator_t::BITWISE_NEGATIVE, { precedence_t::PREFIX, 1, "flip" } },
    { operator_t::MULTIPLY, { precedence_t::PREFIX, 1, "dereference" } },
    { operator_t::AND, { precedence_t::PREFIX, 1, "reference" } },
};
std::map<operator_t, op_data_t> bin_ops {
    { operator_t::INCREASE, { precedence_t::POSTFIX, 1, "inc_post" } },
    { operator_t::DECREASE, { precedence_t::POSTFIX, 1, "dec_post" } },
    { (operator_t)-1, sizeof_data },

    { operator_t::ADD, { precedence_t::ADD, 2, "add" } },
    { operator_t::SUBTRACT, { precedence_t::ADD, 2, "subtract" } },

    { operator_t::MULTIPLY, { precedence_t::MULT, 2, "multiply" } },
    { operator_t::DIVIDE, { precedence_t::MULT, 2, "divide" } },
    { operator_t::MODULO, { precedence_t::MULT, 2, "modulo" } },

    { operator_t::SHIFT_LEFT, { precedence_t::SHIFT, 2, "shl" } },
    { operator_t::SHIFT_RIGHT, { precedence_t::SHIFT, 2, "shr" } },

    { operator_t::LESS_THAN, { precedence_t::COMP, 2, "less" } },
    { operator_t::LESS_THAN_EQUALS, { precedence_t::COMP, 2, "less_eq" } },
    { operator_t::GREATER_THAN, { precedence_t::COMP, 2, "great" } },
    { operator_t::GREATER_THAN_EQUALS, { precedence_t::COMP, 2, "great_eq" } },

    { operator_t::EQUALS, { precedence_t::EQU, 2, "eq" } },
    { operator_t::NOT_EQUALS, { precedence_t::EQU, 2, "neq" } },

    { operator_t::AND, { precedence_t::BIN_AND, 2, "great_eq" } },
    { operator_t::OR, { precedence_t::BIN_OR, 2, "great_eq" } },
    { operator_t::XOR, { precedence_t::BIN_XOR, 2, "great_eq" } },

    { operator_t::DOUBLE_AND, { precedence_t::BOOL_AND, 2, "great_eq" } },
    { operator_t::DOUBLE_OR, { precedence_t::BOOL_OR, 2, "great_eq" } },

    { operator_t::ASSIGN, { precedence_t::ASSIGN, 2, "assign", true } },
    { operator_t::ASSIGN_ADD, { precedence_t::ASSIGN, 2, "assign_add", true } },
    { operator_t::ASSIGN_SUBTRACT, { precedence_t::ASSIGN, 2, "assign_subtract", true } },
    { operator_t::ASSIGN_MULTIPLY, { precedence_t::ASSIGN, 2, "assign_multiply", true } },
    { operator_t::ASSIGN_DIVIDE, { precedence_t::ASSIGN, 2, "assign_divide", true } },
    { operator_t::ASSIGN_MODULO, { precedence_t::ASSIGN, 2, "assign_modulo", true } },
    { operator_t::ASSIGN_SHIFT_LEFT, { precedence_t::ASSIGN, 2, "assign_shl", true } },
    { operator_t::ASSIGN_SHIFT_RIGHT, { precedence_t::ASSIGN, 2, "assign_shr", true } },
    { operator_t::ASSIGN_XOR, { precedence_t::ASSIGN, 2, "assign_xor", true } },
    { operator_t::ASSIGN_AND, { precedence_t::ASSIGN, 2, "assign_and", true } },
    { operator_t::ASSIGN_OR, { precedence_t::ASSIGN, 2, "assign_or", true } },
    { operator_t::ASSIGN_DOUBLE_AND, { precedence_t::ASSIGN, 2, "assign_dand", true } },
    { operator_t::ASSIGN_DOUBLE_OR, { precedence_t::ASSIGN, 2, "assign_dor", true } },
    { operator_t::ASSIGN_NULL_COALESCING, { precedence_t::ASSIGN, 2, "assign_null_coal", true } },
};

map_t op_to_map(located_t<op_data_t> op) {
    return {
        { "$_name", "$_operator" },
        { "ops", array_t() },
        { "location", conv::loc_to_map(op.location) },
        { "op", op.name },
    };
}

bool pop(std::vector<located_t<op_data_t>> &op_stack, array_t &res) {
    if (op_stack.empty()) return false;

    auto map = op_to_map(op_stack.back());
    auto op_n = op_stack.back().op_n;
    auto loc = op_stack.back().location;
    op_stack.pop_back();

    if (res.size() < op_n) return false;

    auto &ops = map["ops"].array();


    for (size_t i = 0; i < op_n; i++) {
        ops.push_back(res.back());
        loc = loc.intersect(conv::map_to_loc(res.back().map()["location"].string()));
        res.pop_back();
    }

    map["location"] = conv::loc_to_map(loc);

    std::reverse(ops.begin(), ops.end());
    res.push_back(map);

    return true;
}
bool pop_paren(std::vector<located_t<op_data_t>> &op_stack, array_t &res) {
    bool has_paren = false;
    for (const auto &op : op_stack) {
        if (op.precedence == precedence_t::PAREN) {
            has_paren = true;
            break;
        }
    }
    if (!has_paren) return false;

    while (true) {
        if (op_stack.back().precedence == precedence_t::PAREN) break;
        if (!pop(op_stack, res)) return false;
    }

    op_stack.pop_back();
    return true;
}
bool pop_call(size_t n, location_t loc, std::vector<located_t<op_data_t>> &op_stack, array_t &res) {
    map_t call = {
        { "$_name", "$_call" },
    };

    array_t &args = call["args"].array({});

    while (true) {
        if (op_stack.back().precedence == precedence_t::CALL_START) break;
        if (!pop(op_stack, res)) return false;
    }
    loc = loc.intersect(op_stack.back().location);
    op_stack.pop_back();
    call["location"] = conv::loc_to_map(loc);

    for (size_t i = 0; i < n; i++) {
        args.push_back(res.back());
        res.pop_back();
    }

    std::reverse(args.begin(), args.end());

    call["func"] = res.back();
    res.pop_back();
    res.push_back(call);

    return true;
}
bool pop_until(const op_data_t &data, tree_helper_t &h, std::vector<located_t<op_data_t>> &op_stack, array_t &res) {
    while (!op_stack.empty()) {
        auto &back_data = op_stack.back();
        if (data.assoc ?
            back_data.precedence >= data.precedence :
            back_data.precedence > data.precedence
        ) break;

        if (!pop(op_stack, res)) return h.err("Expected an expression on the right side of this operator.");
    }
    return true;
}

bool ast::parse_exp_var(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    return ctx.parse(parse_nmsp, res_i, out);
}
bool ast::parse_exp_int_lit(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (h.curr().is_int_literal()) {
        auto &arr = out["content"].array({});
        for (auto b : h.curr().literal()) arr.push_back((float)b);
        out["location"] = conv::loc_to_map(h.loc());
        return h.submit(true);
    }

    return false;
}
bool ast::parse_exp_str_lit(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (h.curr().is_str_literal()) {
        auto &arr = out["content"].array({});
        for (auto b : h.curr().literal()) arr.push_back((float)b);
        out["location"] = conv::loc_to_map(h.loc());
        return h.submit(true);
    }

    return false;
}

bool ast::parse_exp(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    bool last_val = false;
    map_t val;
    std::vector<located_t<op_data_t>> op_stack;
    std::vector<size_t> call_args_n;
    auto res = array_t();


    while (true) {
        if (h.ended()) break;

        if (!last_val && h.curr().is_identifier("sizeof")) {
            op_stack.push_back({ h.loc(), sizeof_data });
            h.advance("Expected a value on the right side of the operator.");
            continue;
        }
        if (!last_val && h.push_parse(ctx.group("$_exp_val"), res)) {
            last_val = true;
            continue;
        }
        if (h.curr().is_operator()) {
            auto op = h.curr()._operator();
            if (last_val) {
                if (op == operator_t::PAREN_OPEN) {
                    h.advance("Expected an argument or closing parens.");
                    op_stack.push_back({ h.loc(), { precedence_t::CALL_START } });
                    if (h.curr().is_operator(operator_t::PAREN_CLOSE)) {
                        pop_call(0, h.loc(), op_stack, res);
                    }
                    else {
                        call_args_n.push_back(1);
                    }
                    last_val = false;
                }
                else if (op == operator_t::PAREN_CLOSE) {
                    bool is_call = false, is_paren = false;

                    for (auto i = op_stack.rbegin(); i != op_stack.rend(); i++) {
                        if (i->precedence == precedence_t::PAREN) {
                            is_paren = true;
                            break;
                        }
                        else if (i->precedence == precedence_t::CALL_START) {
                            is_call = true;
                            break;
                        }
                    }

                    if (is_call) {
                        pop_call(call_args_n.back(), h.loc(), op_stack, res);
                        call_args_n.pop_back();
                    }
                    else if (is_paren) pop_paren(op_stack, res);
                    else break;

                    if (!h.try_advance()) break;
                }
                else if (op == operator_t::COMMA) {
                    if (call_args_n.size() == 0) break;
                    h.advance("Expected an argument.");

                    pop_until({ .precedence = precedence_t::CALL_START, .assoc = true }, h, op_stack, res);
                    call_args_n.back()++;
                    last_val = false;
                }
                else if (op == operator_t::COLON) {
                    h.advance("Expected a type.");
                    pop_until({ .precedence = precedence_t::PREFIX, .assoc = true }, h, op_stack, res);
                    map_t cast = {
                        { "$_name", "$_cast" },
                        { "exp", res.back() },
                    };

                    res.pop_back();
                    h.force_parse(parse_type, "Expected a type.", cast["type"].map({}));
                    cast["location"] = conv::loc_to_map(location_t::intersect(
                        conv::map_to_loc(cast["exp"].map()["location"].string()),
                        conv::map_to_loc(cast["type"].map()["location"].string())
                    ));
                    res.push_back(cast);
                }
                else if (op == operator_t::DOT || op == operator_t::PTR_MEMBER) {
                    h.advance("Expected an identifier.");
                    pop_until({ .precedence = precedence_t::POSTFIX, .assoc = true }, h, op_stack, res);

                    map_t member_access = {
                        { "exp", res.back() },
                        { "is_ptr", op == operator_t::PTR_MEMBER },
                    };
                    h.force_parse(parse_identifier, "Expected an identifier.", member_access["name"].map({}));
                    member_access["location"] = conv::loc_to_map(
                        conv::map_to_loc(member_access["name"].map()["location"].string()).intersect(
                            conv::map_to_loc(res.back().map()["location"].string())
                        )
                    );
                    res.pop_back();
                    res.push_back(member_access);
                }
                else if (bin_ops.find(op) != bin_ops.end()) {
                    auto data = bin_ops[op];
                    pop_until(data, h, op_stack, res);
                    op_stack.push_back({ h.loc(), data });

                    if (data.op_n == 1) {
                        last_val = true;
                        if (!pop(op_stack, res)) return h.err("Expected an expression on the right side of this operator.");
                        if (h.try_advance()) break;
                    }
                    else {
                        last_val = false;
                        h.advance("Expected a value on the right side of the operator.");
                    }
                }
                else break;
            }
            else {
                if (op == operator_t::PAREN_OPEN) {
                    op_stack.push_back({ h.loc(), { precedence_t::PAREN } });
                    h.advance("Expected a value.");
                    last_val = false;
                }
                else if (pre_ops.find(op) != pre_ops.end()) {
                    op_stack.push_back({ h.loc(), pre_ops[op] });
                    h.advance("Expected a value on the right side of the operator.");
                }
                else break;
            }
            continue;
        }
        else break;
    }

    if (res.size() == 0) return false;

    while (!op_stack.empty()) {
        if (op_stack.back().precedence == precedence_t::PAREN) throw message_t::error("Unclosed paren.", op_stack.back().location);
        if (op_stack.back().precedence == precedence_t::CALL_START) throw message_t::error("Unclosed call.", op_stack.back().location);
        if (!pop(op_stack, res)) return h.err("Expected an expression on the right side of this operator.");
    }

    out = res.front().map();

    return h.submit(false);
}
bool ast::parse_stat_exp(ast_ctx_t &ctx, size_t &i, map_t &res) {
    tree_helper_t h(ctx, i);
    if (!h.parse(parse_exp, res)) return false;
    if (!h.ended() && h.curr().is_operator(operator_t::SEMICOLON)) return h.submit(true);

    ctx.messages.push(message_t::error("Expected a semicolon.", h.loc(1)));
    return h.submit(false);
}
