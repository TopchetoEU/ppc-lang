#include "compiler/treeifier/ast.hh"

namespace ppc::comp::tree::ast {
    struct tree_helper_t {
    private:
        ast_ctx_t &ctx;
        size_t &res_i;
        size_t i;
    public:
        void submit() {
            res_i = i;
        }

        bool ended() {
            return i == ctx.tokens.size();
        }

        tok::token_t &curr() { return ctx.tokens[i]; }

        location_t next_loc(size_t n = 1) {
            location_t res = loc();
            res.start += res.length;
            res.code_start += res.length;
            res.length = n;
            return res;
        }
        location_t loc() {
            if (ended()) {
                if (i == 0) return location_t::NONE;

                location_t loc = ctx.tokens[i - 1].location;

                loc.start += loc.length;
                loc.code_start += loc.length;
                loc.length = 1;

                return loc;
            }
            else return curr().location;
        }

        bool try_parse(const parser_t &parser, data::map_t &out, messages::msg_stack_t &messages) {
            try {
                return parser(ctx, i, out);
            }
            catch (messages::message_t msg) {
                messages.push(msg);
                return false;
            }
        }
        bool try_parse(const parser_t &parser, data::map_t &out) {
            try {
                return parser(ctx, i, out);
            }
            catch (messages::message_t msg) {
                return false;
            }
        }

        bool try_advance() {
            if (ended()) return false;
            i++;
            return !ended();
        }
        bool advance() {
            if (ended()) throw messages::message_t(message_t::ERROR, "Unexpected end.", loc());
            i++;
            if (ended()) throw messages::message_t(message_t::ERROR, "Unexpected end.", loc());
        }

        tree_helper_t(ast_ctx_t &ctx, size_t &i): ctx(ctx), res_i(i) {
            this->i = i;
        }
    };
}