#include "compiler/treeifier/lexer.hh"
#include "compiler/treeifier/tokenizer.hh"
#include <algorithm>
#include <string>

using namespace ppc;
using namespace messages;
using namespace comp::tree;
using namespace std::string_literals;

static std::vector<uint8_t> parse_string(msg_stack_t &msg_stack, bool is_char, const lex::token_t &token) {
    char literal_char = is_char ? '\'' : '"';

    bool escaping = false;

    std::vector<uint8_t> res;
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
            else if (c >= '0' && c <= '7') {
                new_c = 0;
                size_t n = 0;
                while (c >= '0' && c <= '7') {
                    new_c <<= 3;
                    new_c |= c - '0';
                    c = token.data[++i];
                    n++;
                }
                if (n > 3) {
                    location_t loc = curr_char_loc;
                    loc.code_start--;
                    loc.start--;
                    loc.length = n + 1;
                    msg_stack.warn("Octal escape sequence overflows 255 8-bit limit (3 digits).", loc);
                }
                curr_char_loc.start += n - 1;
                i--;
            }
            // TODO: Add support for hex and utf8 literals
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

static std::vector<uint8_t> parse_bin(msg_stack_t &msg_stack, size_t i, const std::string &data) {
    std::vector<uint8_t> res;

    int last_byte = 0;
    int lastbyte_n = 0;

    for (size_t j = i; j < data.length(); j++) {
        if (lastbyte_n == 8) {
            lastbyte_n = 0;
            res.push_back(last_byte);
            last_byte = 0;
        }

        last_byte <<= 1;
        last_byte |= data[j] - '0';
        lastbyte_n++;
    }

    res.push_back(last_byte);
    std::reverse(res.begin(), res.end());

    return res;
}
static std::vector<uint8_t> parse_hex(msg_stack_t &msg_stack, size_t i, const std::string &data) {
    std::vector<uint8_t> res;

    int last_byte = 0;
    int lastbyte_n = 0;

    for (size_t j = i; j < data.length(); j++) {
        if (lastbyte_n == 8) {
            lastbyte_n = 0;
            res.push_back(last_byte);
            last_byte = 0;
        }

        int digit = data[j] - '0';
        if (data[j] >= 'a' && data[j] <= 'f') digit = data[j] - 'a' + 10;
        if (data[j] >= 'A' && data[j] <= 'F') digit = data[j] - 'F' + 10;

        last_byte <<= 4;
        last_byte |= digit;
        lastbyte_n += 4;
    }

    res.push_back(last_byte);
    std::reverse(res.begin(), res.end());

    return res;
}
static std::vector<uint8_t> parse_oct(msg_stack_t &msg_stack, size_t i, const std::string &data) {
    std::vector<uint8_t> res;

    int last_byte = 0;
    int lastbyte_n = 0;

    for (size_t j = i; j < data.length(); j++) {
        if (lastbyte_n >= 8) {
            lastbyte_n = 0;
            res.push_back(last_byte);
            last_byte >>= 8;
        }

        int digit = data[j] - '0';

        last_byte <<= 3;
        last_byte |= digit;
        lastbyte_n += 3;
    }

    res.push_back(last_byte);
    std::reverse(res.begin(), res.end());

    return res;
}
static void mult_10(std::vector<uint8_t> &val) {
    std::vector<uint8_t> res;

    int carry = 0;

    for (size_t i = 0; i < val.size(); i++) {
        carry = val[i] * 10 + carry;
        res.push_back(carry);
        carry >>= 8;
    }

    if (carry != 0) res.push_back(carry);
    val = res;
}
static void add_byte(std::vector<uint8_t> &a, uint8_t b) {
    int carry = b;

    for (size_t i = 0; i < a.size(); i++) {
        carry = a[i] + carry;
        a[i] = carry;
        carry >>= 8;
        if (carry == 0) break;
    }

    if (carry != 0) a.push_back(carry);
}
static std::vector<uint8_t> parse_dec(msg_stack_t &msg_stack, size_t i, const std::string &data) {
    std::vector<uint8_t> res;

    for (size_t j = i; j < data.length(); j++) {
        int digit = data[j] - '0';

        mult_10(res);
        if (res.empty()) res.push_back(digit);
        else add_byte(res, digit);
    }

    return res;
}
static std::vector<uint8_t> parse_float(msg_stack_t &msg_stack, const lex::token_t &token) {
    throw "no floats lol bozo"s;
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
            return { parse_bin(msg_stack, 1, in.data), false, in.location };
        case lex::token_t::OCT_LITERAL:
            return { parse_oct(msg_stack, 1, in.data), false, in.location };
        case lex::token_t::DEC_LITERAL:
            return { parse_dec(msg_stack, 0, in.data), false, in.location };
        case lex::token_t::HEX_LITERAL:
            return { parse_hex(msg_stack, 2, in.data), false, in.location };
        case lex::token_t::FLOAT_LITERAL:
            return { parse_float(msg_stack, in), false, in.location };
        case lex::token_t::STRING_LITERAL:
            return { parse_string(msg_stack, false, in), true, in.location };
        case lex::token_t::CHAR_LITERAL: {
            auto res = parse_string(msg_stack, true, in);
            std::reverse(res.begin(), res.end());
            return { res, false, in.location };
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
