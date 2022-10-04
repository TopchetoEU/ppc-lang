#include <string>
#include "compiler/treeifier/tokenizer.hh"
#include "compiler/treeifier/lexer.hh"

using namespace ppc;
using namespace messages;
using namespace comp::tree;
using namespace std::string_literals;

static std::vector<char> parse_string(msg_stack_t &msg_stack, bool is_char, lex::token_t token) {
    char literal_char = is_char ? '\'' : '"';

    bool escaping = false;

    std::vector<char> res;
    location_t curr_char_loc = token.location;
    curr_char_loc.length = 1;
    curr_char_loc.start++;

    char c;

    for (std::size_t i = 1; (c = token.data[i]); i++) {
        if (escaping) {
            char new_c = c;

            if (c == 'b') new_c = '\b';
            else if (c == 'a') new_c = '\a';
            else if (c == 'e') new_c = '\e';
            else if (c == 'f') new_c = '\f';
            else if (c == 'n') new_c = '\n';
            else if (c == 'r') new_c = '\r';
            else if (c == 't') new_c = '\t';
            else if (c == 'v') new_c = '\v';
            // TODO: Add support for oct, hex and utf8 literals
            else if (c == literal_char || c == '\\') new_c = c;
            else {
                throw message_t(message_t::ERROR, "Unescapable character.", curr_char_loc);
            }
            res.push_back(new_c);
            escaping = false;
        }
        else {
            if (c == '\\') escaping = true;
            else if (c == literal_char) return res;
            else res.push_back(c);
        }

        curr_char_loc.start++;
        if (c == '\n') break;
    }

    if (is_char) throw message_t(message_t::ERROR, "Unterminated char literal.", token.location);
    else throw message_t(message_t::ERROR, "Unterminated string literal.", token.location);
}
static token_t parse_int(msg_stack_t &msg_stack, lex::token_t token) {
    enum radix_t {
        BINARY,
        OCTAL,
        DECIMAL,
        HEXADECIMAL,
    } radix;

    std::size_t i = 0;

    switch (token.type) {
        case lex::token_t::BIN_LITERAL:
            i += 2;
            radix = BINARY;
            break;
        case lex::token_t::OCT_LITERAL:
            i++;
            radix = OCTAL;
            break;
        case lex::token_t::DEC_LITERAL:
            radix = DECIMAL;
            break;
        case lex::token_t::HEX_LITERAL:
            i += 2;
            radix = HEXADECIMAL;
            break;
        default:
            throw "WTF r u doing bro?"s;
    }

    std::size_t j = token.data.length() - 1;

    uint64_t res = 0;

    for (; i <= j; i++) {
        char c = token.data[i];
        int8_t digit;
        switch (radix) {
            case BINARY:
                digit = c - '0';
                res <<= 1;
                res |= digit;
                break;
            case OCTAL:
                digit = c - '0';
                if (digit < 0 || digit > 7) {
                    throw message_t(message_t::ERROR, "Octal literals may contain numbers between 0 and 7.", token.location);
                }
                res <<= 3;
                res |= digit;
                break;
            case 2:
                digit = c - '0';
                res *= 10;
                res += digit;
                break;
            case 3:
                if (c >= 'a' && c <= 'f') digit = c - 'a' + 9;
                else if (c >= 'A' && c <= 'F') digit = c - 'A' + 9;
                else if (c >= '0' && c <= '9') digit = c - '0';
                else throw message_t(message_t::ERROR, "Invalid character '"s + c + "' in hex literal.", token.location);
                res <<= 4;
                res |= digit;
                break;
        }
    }

    return token_t(res, token.location);
}
static token_t parse_float(msg_stack_t &msg_stack, lex::token_t token) {
    double whole = 0, fract = 0;

    char c;
    std::size_t i;

    for (i = 0; i < token.data.length() && ((c = token.data[i]) > '0' && c < '9'); i++) {
        if (c == '.') break;
        int digit = c - '0';
        whole *= 10;
        whole += digit;
    }

    if (c == '.') {
        i++;
        for (; i < token.data.length() && ((c = token.data[i]) > '0' && c < '9'); i++) {
            int digit = c - '0';
            fract += digit;
            fract /= 10;
        }
    }

    return token_t(whole + fract, token.location);
}

token_t token_t::parse(messages::msg_stack_t &msg_stack, lex::token_t in) {
    switch (in.type) {
        case lex::token_t::IDENTIFIER:
            return token_t(in.data, in.location);
        case lex::token_t::OPERATOR:
            try {
                auto op = operator_find(in.data);
                return token_t(op, in.location);
            }
            catch (std::string &err) {
                throw message_t(message_t::ERROR, "Operator not recognised."s, in.location);
            }
        case lex::token_t::BIN_LITERAL:
        case lex::token_t::OCT_LITERAL:
        case lex::token_t::DEC_LITERAL:
        case lex::token_t::HEX_LITERAL:
            return parse_int(msg_stack, in);
        case lex::token_t::FLOAT_LITERAL:
            return parse_float(msg_stack, in);
        case lex::token_t::STRING_LITERAL:
            return { parse_string(msg_stack, false, in) };
        case lex::token_t::CHAR_LITERAL: {
            auto str = parse_string(msg_stack, true, in);
            if (str.size() != 1) throw message_t(message_t::ERROR, "Char literal must consist of just one character.", in.location);
            return str.front();
        }
        default:
            throw message_t(message_t::ERROR, "Token type not recognised.", in.location);
    }
}
std::vector<token_t> token_t::parse_many(messages::msg_stack_t &msg_stack, std::vector<lex::token_t> tokens) {
    std::vector<token_t> res;

    for (auto &tok : tokens) {
        res.push_back(token_t::parse(msg_stack, tok));
    }

    return res;
}
