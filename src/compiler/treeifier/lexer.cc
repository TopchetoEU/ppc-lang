#include <sstream>
#include "compiler/treeifier/lexer.hh"
#include "utils/message.hh"

using namespace ppc;
using namespace comp::tree::lex;

struct lexlet_t {
    bool(*is_valid)(char curr);
    struct process_res_t {
        bool ended;
        bool repeat;
        bool dont_add;
        const lexlet_t *new_parselet;
        bool has_message;
        messages::message_t msg;
    };
    process_res_t (*process)(char curr);
    token_t::kind_t type;
};

using process_res_t = lexlet_t::process_res_t;

extern const lexlet_t LEXLET_DEFAULT;
extern const lexlet_t LEXLET_IDENTIFIER;
extern const lexlet_t LEXLET_OPERATOR;
extern const lexlet_t LEXLET_ZERO;
extern const lexlet_t LEXLET_FLOAT;
extern const lexlet_t LEXLET_BIN;
extern const lexlet_t LEXLET_OCT;
extern const lexlet_t LEXLET_DEC;
extern const lexlet_t LEXLET_HEX;
extern const lexlet_t LEXLET_STRING_LITERAL;
extern const lexlet_t LEXLET_CHAR_LITERAL;
extern const lexlet_t LEXLET_COMMENT;
extern const lexlet_t LEXLET_MULTICOMMENT;

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}
static bool is_oct(char c) {
    return c >= '0' && c <= '7';
}
static bool is_hex(char c) {
    return is_digit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' || c <= 'f');
}
static bool is_lower(char c) {
    return c >= 'a' && c <= 'z';
}
static bool is_upper(char c) {
    return c >= 'A' && c <= 'Z';
}
static bool is_letter(char c) {
    return is_lower(c) || is_upper(c);
}
static bool is_alphanumeric(char c) {
    return is_letter(c) || is_digit(c);
}
static bool is_any(char c, std::string chars) {
    return chars.find(c) != -1u;
}

static process_res_t lexer_switch(const lexlet_t *lexlet) {
    return {
        .ended = false,
        .repeat = false,
        .new_parselet = lexlet,
    };
}
static process_res_t lexer_repeat_switch(const lexlet_t *lexlet) {
    return (process_res_t) {
        .ended = false,
        .repeat = true,
        .new_parselet = lexlet,
    };
}
static process_res_t lexer_end() {
    return (process_res_t) {
        .ended = true,
        .repeat = true,
        .new_parselet = nullptr,
    };
}
static process_res_t lexer_none() {
    return (process_res_t) {
        .ended = false,
        .repeat = false,
        .new_parselet = nullptr,
    };
}
static process_res_t default_process(char curr) {
    if (LEXLET_STRING_LITERAL.is_valid(curr)) return lexer_switch(&LEXLET_STRING_LITERAL);
    if (LEXLET_CHAR_LITERAL.is_valid(curr)) return lexer_switch(&LEXLET_CHAR_LITERAL);
    if (LEXLET_OPERATOR.is_valid(curr)) return lexer_switch(&LEXLET_OPERATOR);
    if (LEXLET_ZERO.is_valid(curr)) return lexer_switch(&LEXLET_ZERO);
    if (LEXLET_DEC.is_valid(curr)) return lexer_switch(&LEXLET_DEC);
    if (LEXLET_FLOAT.is_valid(curr)) return lexer_switch(&LEXLET_FLOAT);
    if (LEXLET_IDENTIFIER.is_valid(curr)) return lexer_switch(&LEXLET_IDENTIFIER);
    else return (process_res_t) {
        .ended = true,
        .repeat = false,
        .new_parselet = nullptr,
    };
}

static bool identifier_is_valid(char curr) {
    return is_letter(curr) || curr == '_' || curr == '@' || curr == '$';
}
static process_res_t identifier_process(char curr) {
    bool valid = (is_alphanumeric(curr) || curr == '_' || curr == '@' || curr == '$');
    return (process_res_t) {
        .ended = !valid,
        .repeat = !valid,
        .new_parselet = &LEXLET_IDENTIFIER,
    };
}

static bool last_escape = false;
static bool literal_ended = false;

static bool string_is_valid(char curr) {
    last_escape = false;
    literal_ended = false;
    return curr == '"';
}
static process_res_t string_process(char curr) {
    if (last_escape) {
        last_escape = false;
        return lexer_none();
    }

    if (curr == '\\') {
        last_escape = true;
    }
    else if (curr == '"') {
        literal_ended = true;
    }
    else if (literal_ended) return lexer_end();
    return lexer_none();
}

static bool char_is_valid(char curr) {
    last_escape = false;
    literal_ended = false;
    return curr == '\'';
}
static process_res_t char_process(char curr) {
    if (last_escape) {
        last_escape = false;
        return lexer_none();
    }

    if (curr == '\\') {
        last_escape = true;
    }
    else if (curr == '\'') {
        literal_ended = true;
    }
    else if (literal_ended) return lexer_end();
    return lexer_none();
}

static char first_op;
static int op_i = 0;

static bool operator_is_valid(char curr) {
    if (is_any(curr, "=!<>+-*/%&|^?:,.(){}[];")) {
        first_op = curr;
        op_i = 1;
        return true;
    }
    else return false;
}
static process_res_t operator_process(char curr) {
    bool failed = true;
    if (first_op == curr && op_i == 1 && is_any(curr, "+-&|?<>")) failed = false;
    if (curr == '=') {
        if (op_i == 1 && is_any(first_op, "<>=!+-/*%")) failed = false;
        if (op_i == 2 && is_any(first_op, "<>?")) failed = false;
    }
    if (first_op == '-' && curr == '>' && op_i == 1) failed = false;

    if (first_op == '/' && op_i == 1) {
        if (curr == '/') return lexer_switch(&LEXLET_COMMENT);
        else if (curr == '*') return lexer_switch(&LEXLET_MULTICOMMENT);
    }

    op_i++;

    if (failed) return lexer_end();
    else return lexer_none();
}

static bool zero_is_valid(char curr) {
    return curr == '0';
}
static process_res_t zero_process(char curr) {
    if (curr == '.') return lexer_switch(&LEXLET_FLOAT);
    else if (curr == 'b') return lexer_switch(&LEXLET_BIN);
    else if (curr == 'x') return lexer_switch(&LEXLET_HEX);
    else if (is_digit(curr)) return lexer_repeat_switch(&LEXLET_OCT);
    else return lexer_end();
}

static bool dec_is_valid(char curr) {
    return is_digit(curr);
}
static process_res_t dec_process(char curr) {
    if (is_digit(curr)) return lexer_none();
    else if (curr == '.') return lexer_switch(&LEXLET_FLOAT);
    else return lexer_end();
}

static bool only_dot = false;

static bool float_is_valid(char curr) {
    return only_dot = curr == '.';
}
static process_res_t float_process(char curr) {
    if (is_digit(curr)) {
        only_dot = false;
        return lexer_none();
    }
    else return lexer_end();
}

static process_res_t hex_process(char curr) {
    if (is_hex(curr)) return lexer_none();
    else return lexer_end();
}
static process_res_t bin_process(char curr) {
    if (curr == '0' || curr == '1') return lexer_none();
    else if (is_digit(curr))
        throw messages::message_t {  messages::message_t::ERROR, NO_LOCATION, "A binary literal may only contain zeroes and ones." };
    else return lexer_end();
}
static process_res_t oct_process(char curr) {
    if (is_oct(curr)) return lexer_none();
    else if (is_digit(curr))
        throw messages::message_t { messages::message_t::ERROR, NO_LOCATION, "An octal literal may only contain octal digits." };
    else return lexer_end();
}

static process_res_t comment_process(char curr) {
    if (curr == '\n') return lexer_end();
    else return (process_res_t) {
        .ended = false,
        .dont_add = true,
    };
}

static bool last_star = false;

static process_res_t multicomment_process(char curr) {
    if (curr == '/' && last_star) {
        last_star = false;
        return {
            .ended = true,
            .repeat = false,
            .new_parselet = nullptr,
        };
    }
    if (curr == '*') last_star = true;
    
    return {
        .ended = false,
        .dont_add = true,
    };
}

const lexlet_t LEXLET_DEFAULT = (lexlet_t) {
    .process = default_process,
    .type = token_t::NONE,
};
const lexlet_t LEXLET_IDENTIFIER = (lexlet_t) {
    .is_valid = identifier_is_valid,
    .process = identifier_process,
    .type = token_t::IDENTIFIER,
};
const lexlet_t LEXLET_ZERO = (lexlet_t) {
    .is_valid = zero_is_valid,
    .process = zero_process,
    .type = token_t::DEC_LITERAL,
};
const lexlet_t LEXLET_DEC = (lexlet_t) {
    .is_valid = dec_is_valid,
    .process = dec_process,
    .type = token_t::DEC_LITERAL,
};
const lexlet_t LEXLET_HEX = (lexlet_t) {
    .process = hex_process,
    .type = token_t::HEX_LITERAL,
};
const lexlet_t LEXLET_BIN = (lexlet_t) {
    .process = bin_process,
    .type = token_t::BIN_LITERAL,
};
const lexlet_t LEXLET_OCT = (lexlet_t) {
    .process = oct_process,
    .type = token_t::OCT_LITERAL,
};
const lexlet_t LEXLET_FLOAT = (lexlet_t) {
    .is_valid = float_is_valid,
    .process = float_process,
    .type = token_t::FLOAT_LITERAL,
};
const lexlet_t LEXLET_OPERATOR = (lexlet_t) {
    .is_valid = operator_is_valid,
    .process = operator_process,
    .type = token_t::OPERATOR,
};
const lexlet_t LEXLET_STRING_LITERAL = (lexlet_t) {
    .is_valid = string_is_valid,
    .process = string_process,
    .type = token_t::STRING_LITERAL,
};
const lexlet_t LEXLET_CHAR_LITERAL = (lexlet_t) {
    .is_valid = char_is_valid,
    .process = char_process,
    .type = token_t::CHAR_LITERAL,
};
const lexlet_t LEXLET_COMMENT = {
    .is_valid = nullptr,
    .process = comment_process,
    .type = token_t::NONE,
};
const lexlet_t LEXLET_MULTICOMMENT = {
    .is_valid = nullptr,
    .process = multicomment_process,
    .type = token_t::NONE,
};

std::vector<token_t> token_t::parse_many(ppc::messages::msg_stack_t &msg_stack, const std::string &filename, const std::string &src) {
    std::vector<token_t> tokens;
    std::vector<char> curr_token;
    lexlet_t curr = LEXLET_DEFAULT;
    std::size_t start = 0, line = 0, curr_start = 0, curr_line = 0, length = 0, i = 0;

    while (src[i]) {
        char c = src[i];
        try {
            process_res_t res = curr.process(c);
            if (i == 0) res.repeat = false;
            if (res.has_message) throw res.msg;

            if (res.ended) {
                if (curr.type) {
                    location_t loc = { filename, line, start, i - length, length };
                    tokens.push_back({ curr.type, { curr_token.begin(), curr_token.end() }, loc });
                    curr_token.clear();
                }

                length = 0;

                curr = LEXLET_DEFAULT;
            }
            else {
                if (res.new_parselet) {
                    if (!curr.type) {
                        start = curr_start;
                        line = curr_line;
                    }
                    curr = *res.new_parselet;
                }
                if (!res.dont_add) {
                    curr_token.push_back(c);
                    length++;
                }
            }

            if (!res.repeat) {
                curr_start++;
                if (c == '\n') {
                    curr_line++;
                    curr_start = 0;
                }
                i++;
            }
        }
        catch (const messages::message_t &msg) { 
            throw messages::message_t { msg.level, { filename, line, start, i - length, length }, msg.content };
        }
    }

    location_t loc = { filename, line, start, i - length, length };
    if (curr.type) {
        tokens.push_back(token_t {
            curr.type, std::string { curr_token.begin(), curr_token.end() },
            { filename, line, start, i - length, length }
        });
    }

    return tokens;
}
std::vector<token_t> token_t::parse_file(ppc::messages::msg_stack_t &msg_stack, const std::string &filename, std::istream &f) {
    std::vector<char> contents;
    int c;

    while ((c = f.get()) != EOF) contents.push_back(c);

    return parse_many(msg_stack, filename, std::string { contents.begin(), contents.end() });
}
