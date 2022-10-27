#pragma once

#include "utils/location.hh"
#include "utils/message.hh"
#include "compiler/treeifier/lexer.hh"

namespace ppc::comp::tree {
    enum operator_t {
        NONE,

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
            LITERAL,
        } kind;
        union data_t {
            std::string *identifier;
            operator_t _operator;
            std::vector<uint8_t> *literal;
        } data;
    public:
        ppc::location_t location;

        bool is_identifier() const { return kind == IDENTIFIER; }
        bool is_operator() const { return kind == OPERATOR; }
        bool is_literal() const { return kind == LITERAL; }

        const auto &identifier() const {
            if (!is_identifier()) throw std::string { "Token is not an identifier." };
            else return *data.identifier;
        }
        auto _operator() const {
            if (!is_operator()) throw std::string { "Token is not an operator." };
            else return data._operator;
        }
        const auto &literal() const {
            if (!is_literal()) throw std::string { "Token is not a literal." };
            else return *data.literal;
        }

        bool is_operator(operator_t op) const { return is_operator() && _operator() == op; }
        bool is_identifier(const std::string &val) const { return is_identifier() && identifier() == val; }

        token_t() { kind = NONE; }
        token_t(const std::string &identifier, location_t loc = location_t::NONE): location(loc) {
            kind = IDENTIFIER;
            data.identifier = new std::string { identifier };
        }
        token_t(operator_t op, location_t loc = location_t::NONE): location(loc) {
            kind = OPERATOR;
            data._operator = op;
        }
        token_t(const std::vector<uint8_t> &val, location_t loc = location_t::NONE): location(loc) {
            kind = LITERAL;
            data.literal = new std::vector<uint8_t> { val };
        }
        token_t(const token_t &tok): location(tok.location) {
            kind = tok.kind;
            switch (kind) {
                case NONE: break;
                case IDENTIFIER: data.identifier = new std::string { *tok.data.identifier }; break;
                case OPERATOR: data._operator = tok.data._operator; break;
                case LITERAL: data.literal = new std::vector<uint8_t> { *tok.data.literal }; break;
            }
        }
        
        ~token_t() {
            switch (kind) {
                case IDENTIFIER: delete data.identifier; break;
                case LITERAL: delete data.literal; break;
                default: break;
            }
        }

        static token_t parse(messages::msg_stack_t &msg_stack, lex::token_t token);
        static std::vector<token_t> parse_many(messages::msg_stack_t &msg_stack, std::vector<lex::token_t> tokens);
    };

    operator_t operator_find(const std::string &text);
    const std::string &operator_stringify(operator_t kw);
}
