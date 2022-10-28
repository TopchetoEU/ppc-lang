#include <string>
#include "compiler/treeifier/tokenizer.hh"

using namespace ppc::comp::tree;
using namespace ppc::comp;
using namespace std::string_literals;


std::vector<std::string> operators = {
    "(none)",
    "<", ">", "<=", ">=", "==", "!=", "&&", "||",
    "<<", ">>", "^", "&", "|", "!", "~",
    "++", "--",
    "+", "-", "/", "*", "%",
    "?", "??",
    "=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "^=", "&=", "|=", "&&=", "||=", "??=",
    "->", ".", ",", ";", ":", "::",
    "=>",
    "[", "]", "{", "}", "(", ")"
};

const std::string &tree::operator_stringify(operator_t op) {
    if (op < 0 || op >= operators.size()) throw "Invalid operator ID given."s;
    return operators[op];
}
operator_t tree::operator_find(const std::string &raw) {
    std::size_t i = 0;
    for (const auto &op : operators) {
        if (op == raw) return (operator_t)i;
        i++;
    }
    throw "Invalid operator '" + raw + "' given.";
}
