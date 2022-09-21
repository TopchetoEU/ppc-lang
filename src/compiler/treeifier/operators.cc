#include <string>
#include "compiler/treeifier/tokenizer.hh"

using namespace ppc::comp::tree;
using namespace std::string_literals;


std::vector<std::string> operators = {
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


const std::string &tok::operator_stringify(tok::operator_t kw) {
    if (kw < 0 || kw >= operators.size()) throw "Invalid operator ID given."s;
    return operators[kw];
}
tok::operator_t tok::operator_find(const std::string &raw) {
    std::size_t i = 0;
    for (const auto &op : operators) {
        if (op == raw) return (tok::operator_t)i;
        i++;
    }
    throw "Invalid operator '"s + raw + "' given.";
}
