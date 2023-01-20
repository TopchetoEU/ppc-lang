#include "compiler/treeifier/lexer.hh"
#include "utils/message.hh"
#include <sstream>

using namespace ppc;
using namespace ppc::messages;
using namespace ppc::comp::tree::lex;

struct res_t;
using lexlet_t = res_t (*)(char c, std::vector<char> &tok);

struct res_t {
    lexlet_t new_parselet;
    token_t::kind_t type;
    bool _repeat;
    bool _add;

    res_t add(bool val = true) {
        this->_add = val;
        return *this;
    }
    res_t repeat(bool val = true) {
        this->_repeat = val;
        return *this;
    }
};


static inline bool isoct(char c) {
    return c >= '0' && c <= '7';
}
static inline bool is_any(char c, std::string chars) {
    auto res = chars.find(c) != std::string::npos;
    return res;
}
static inline bool is_operator(char c) {
    return is_any(c, "=!<>+-*/%&|^?:,.(){}[];~");
}

static res_t lexlet_default(char c, std::vector<char> &tok);

static res_t lexer_switch(lexlet_t lexlet, bool repeat = false) {
    return {
        .new_parselet = lexlet,
        ._repeat = repeat,
    };
}
static res_t lexer_end(token_t::kind_t type, bool repeat = true) {
    return {
        .new_parselet = lexlet_default,
        .type = type,
        ._repeat = repeat
    };
}
static res_t lexer_none() {
    return { ._add = true };
}

static res_t lexlet_identifier(char c, std::vector<char> &tok) {
    if (is_operator(c) || isspace(c)) return lexer_end(token_t::IDENTIFIER);
    else return lexer_none();
};
static res_t lexlet_hex(char c, std::vector<char> &tok) {
    if (isxdigit(c)) return lexer_none();
    else return lexer_end(token_t::HEX_LITERAL);
};
static res_t lexlet_bin(char c, std::vector<char> &tok) {
    if (is_any(c, "01")) return lexer_none();
    else if (isdigit(c)) throw message_t::error("A binary literal may only contain zeroes and ones.");
    else return lexer_end(token_t::BIN_LITERAL);
};
static res_t lexlet_oct(char c, std::vector<char> &tok) {
    if (isoct(c)) return lexer_none();
    else if (isdigit(c)) throw message_t::error("An octal literal may only contain octal digits.");
    else return lexer_end(token_t::OCT_LITERAL);
};
static res_t lexlet_float(char c, std::vector<char> &tok) {
    if (isdigit(c)) return lexer_none();
    else return lexer_end(token_t::FLOAT_LITERAL);
};
static res_t lexlet_dec(char c, std::vector<char> &tok) {
    if (isdigit(c)) return lexer_none();
    else if (c == '.') return lexer_switch(lexlet_float);
    else return lexer_end(token_t::DEC_LITERAL);
};

static res_t lexlet_zero(char c, std::vector<char> &tok) {
    if (c == '.') return lexer_switch(lexlet_float).add();
    else if (c == 'b') return lexer_switch(lexlet_bin).add();
    else if (c == 'x') return lexer_switch(lexlet_hex).add();
    else if (isdigit(c)) return lexer_switch(lexlet_oct, true);
    else return lexer_end(token_t::DEC_LITERAL);
};
static res_t lexlet_comment(char c, std::vector<char> &tok) {
    tok.clear();
    if (c == '\n') return lexer_switch(lexlet_default);
    else return lexer_none().add(false);
};
static res_t lexlet_multicomment(char c, std::vector<char> &tok) {
    if (c == '/' && tok.size() && tok.back() == '*') {
        tok.clear();
        return lexer_switch(lexlet_default);
    }

    return lexer_none();
};
static res_t lexlet_operator(char c, std::vector<char> &tok) {
    bool failed = false;


    if (tok.size() > 0) {
        failed = true;
        char first_op = tok[0];
        size_t op_i = tok.size();

        if (first_op == '.' && isdigit(c)) return lexer_switch(lexlet_float).add();

        if (first_op == c && op_i == 1 && is_any(c, ":+-&|?<>")) failed = false;
        if (c == '=') {
            if (op_i == 1 && is_any(first_op, "<>=!+-/*%")) failed = false;
            if (op_i == 2 && is_any(first_op, "<>?")) failed = false;
        }
        if (first_op == '-' && c == '>' && op_i == 1) failed = false;

        if (first_op == '/' && op_i == 1) {
            if (c == '/') return lexer_switch(lexlet_comment);
            else if (c == '*') return lexer_switch(lexlet_multicomment);
        }
    }

    if (failed) return lexer_end(token_t::OPERATOR);
    else return lexer_none();
};
static res_t lexlet_string(char c, std::vector<char> &tok) {
    if ((c == '"') && (tok.back() != '\\')) return lexer_end(token_t::STRING_LITERAL, false).add();
    else return lexer_none();
};
static res_t lexlet_char(char c, std::vector<char> &tok) {
    if ((c == '\'') && (tok.back() != '\\')) return lexer_end(token_t::CHAR_LITERAL, false).add();
    else return lexer_none();
};

static res_t lexlet_default(char c, std::vector<char> &tok) {
    if (c == '"') return lexer_switch(lexlet_string).add();
    if (c == '\'') return lexer_switch(lexlet_char).add();
    if (c == '0') return lexer_switch(lexlet_zero).add();
    if (is_operator(c)) return lexer_switch(lexlet_operator).add();
    if (isdigit(c)) return lexer_switch(lexlet_dec).add();
    if (isspace(c)) {
        tok.clear();
        return lexer_none().add(false);
    }
    return lexer_switch(lexlet_identifier).add();
};

std::vector<token_t> token_t::parse_many(ppc::messages::msg_stack_t &msg_stack, const std::string &filename, const std::string &_src) {
    auto src = _src + '\n';
    std::vector<token_t> tokens;
    std::vector<char> curr_token;
    lexlet_t curr = lexlet_default;
    std::size_t start = 0, line = 0, curr_start = 0, curr_line = 0, i = 0;

    while (i < src.size()) {
        char c = src[i];
        try {
            res_t res = curr(c, curr_token);
            if (i == 0) res._repeat = false;

            if (res._add) {
                curr_token.push_back(c);
            }
            if (res.type) {
                size_t len = curr_token.size();
                location_t loc(filename, line, start, i - len, len);
                tokens.push_back({ res.type, { curr_token.begin(), curr_token.end() }, loc });
                curr_token.clear();
            }
            if (res.new_parselet) {
                if (curr == lexlet_default && res.new_parselet != lexlet_default) {
                    start = curr_start;
                    line = curr_line;
                }
                curr = res.new_parselet;
            }

            if (!res._repeat) {
                i++;
                curr_start++;
                if (i == src.size()) break;
                if (c == '\n') {
                    curr_start = 0;
                    curr_line++;
                }
            }
        }
        catch (const messages::message_t &msg) {
            throw message_t(msg.level, msg.content, location_t(filename, line, start, i - curr_token.size(), curr_token.size()));
        }
    }

    curr_start--;

    if (curr_token.size()) curr_token.pop_back();

    if (curr == lexlet_string)
        throw message_t::error("Unclosed string literal.", location_t(filename, line, start, i - curr_token.size(), curr_token.size()));
    if (curr == lexlet_char)
        throw message_t::error("Unclosed char literal.", location_t(filename, line, start, i - curr_token.size(), curr_token.size()));
    if (curr != lexlet_default) throw message_t::error("Unexpected end.", location_t(filename, curr_line, curr_start, i, 1));

    return tokens;
}
std::vector<token_t> token_t::parse_file(ppc::messages::msg_stack_t &msg_stack, const std::string &filename, std::istream &f) {
    std::vector<char> contents;
    int c;

    while ((c = f.get()) != EOF)
        contents.push_back(c);

    return parse_many(msg_stack, filename, std::string { contents.begin(), contents.end() });
}
