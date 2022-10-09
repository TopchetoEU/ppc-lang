#pragma once

#include "utils/location.hh"
#include "utils/message.hh"
#include "compiler/treeifier/lexer.hh"

namespace ppc::comp::tree {
    enum operator_t {
        LESS_THAN,
        GREATER_THAN,
        LESS_THAN_EQUALS,
        GREATER_THAN_EQUALS,
        EQUALS,
        NOT_EQUALS,
        DOUBLE_AND,
        DOUBLE_OR,

        SHIFT_LEFT,
        SHIFT_RIGHT,
        XOR,
        AND,
        OR,
        NOT,
        BITWISE_NEGATIVE,

        INCREASE,
        DECREASE,

        ADD,
        SUBTRACT,
        DIVIDE,
        MULTIPLY,
        MODULO,

        CONDITIONAL,
        NULL_COALESCING,

        ASSIGN,
        ASSIGN_ADD,
        ASSIGN_SUBTRACT,
        ASSIGN_MULTIPLY,
        ASSIGN_DIVIDE,
        ASSIGN_MODULO,
        ASSIGN_SHIFT_LEFT,
        ASSIGN_SHIFT_RIGHT,
        ASSIGN_XOR,
        ASSIGN_AND,
        ASSIGN_OR,
        ASSIGN_DOUBLE_AND,
        ASSIGN_DOUBLE_OR,
        ASSIGN_NULL_COALESCING,

        PTR_MEMBER,
        DOT,
        COMMA,
        SEMICOLON,
        COLON,
        DOUBLE_COLON,
        
        LAMBDA,
        
        BRACKET_OPEN,
        BRACKET_CLOSE,
        BRACE_OPEN,
        BRACE_CLOSE,
        PAREN_OPEN,
        PAREN_CLOSE,

        VAL,
        REF,
        SIZEOF,
    };

    struct token_t {
    private:
        enum kind_t {
            NONE,
            IDENTIFIER,
            OPERATOR,
            INT,
            FLOAT,
            CHAR,
            STRING,
        } kind;
        union data_t {
            std::string *identifier;
            operator_t _operator;
            std::uint64_t int_literal;
            double float_literal;
            char char_literal;
            std::vector<char> *string_literal;
        } data;
    public:
        ppc::location_t location;

        bool is_identifier() { return kind == IDENTIFIER; }
        bool is_operator() { return kind == OPERATOR; }
        bool is_int_lit() { return kind == INT; }
        bool is_float_lit() { return kind == FLOAT; }
        bool is_char_lit() { return kind == CHAR; }
        bool is_string_lit() { return kind == STRING; }

        const auto &identifier() {
            if (!is_identifier()) throw std::string { "Token is not an identifier." };
            else return *data.identifier;
        }
        auto _operator() {
            if (!is_operator()) throw std::string { "Token is not an operator." };
            else return data._operator;
        }
        auto int_lit() {
            if (!is_int_lit()) throw std::string { "Token is not an int literal." };
            else return data.int_literal;
        }
        auto float_lit() {
            if (!is_float_lit()) throw std::string { "Token is not a float literal." };
            else return data.float_literal;
        }
        auto char_lit() {
            if (!is_char_lit()) throw std::string { "Token is not a char literal." };
            else return data.char_literal;
        }
        const auto &string_lit() {
            if (!is_string_lit()) throw std::string { "Token is not a string literal." };
            else return *data.string_literal;
        }

        bool is_operator(operator_t op) { return is_operator() && _operator() == op; }
        bool is_identifier(std::string &&val) { return is_identifier() && identifier() == val; }

        token_t() { kind = NONE; }
        token_t(const std::string &identifier, location_t loc = location_t::NONE): location(loc) {
            kind = IDENTIFIER;
            data.identifier = new std::string { identifier };
        }
        token_t(operator_t op, location_t loc = location_t::NONE): location(loc) {
            kind = OPERATOR;
            data._operator = op;
        }
        token_t(std::uint64_t val, location_t loc = location_t::NONE): location(loc) {
            kind = INT;
            data.int_literal = val;
        }
        token_t(double val, location_t loc = location_t::NONE): location(loc) {
            kind = FLOAT;
            data.float_literal = val;
        }
        token_t(char c, location_t loc = location_t::NONE): location(loc) {
            kind = CHAR;
            data.char_literal = c;
        }
        token_t(const std::vector<char> &val, location_t loc = location_t::NONE): location(loc) {
            kind = STRING;
            data.string_literal = new std::vector<char> { val };
        }
        token_t(const token_t &tok): location(tok.location) {
            kind = tok.kind;
            switch (kind) {
                case NONE: break;
                case IDENTIFIER: data.identifier = new std::string { *tok.data.identifier }; break;
                case OPERATOR: data._operator = tok.data._operator; break;
                case INT: data.int_literal = tok.data.int_literal; break;
                case FLOAT: data.float_literal = tok.data.float_literal; break;
                case CHAR: data.char_literal = tok.data.char_literal; break;
                case STRING: data.string_literal = new std::vector<char> { *tok.data.string_literal }; break;
            }
        }
        
        ~token_t() {
            switch (kind) {
                case IDENTIFIER: delete data.identifier; break;
                case STRING: delete data.string_literal; break;
                default: break;
            }
        }

        static token_t parse(messages::msg_stack_t &msg_stack, lex::token_t token);
        static std::vector<token_t> parse_many(messages::msg_stack_t &msg_stack, std::vector<lex::token_t> tokens);
    };

    operator_t operator_find(const std::string &text);
    const std::string &operator_stringify(operator_t kw);
}
