#include "compiler/treeifier/ast.hh"

namespace ppc::comp::tree::ast {
    struct tree_helper_t {
    private:
        ast_ctx_t &ctx;
        size_t &res_i;

        void throw_ended() {
            if (ended()) throw messages::message_t(message_t::ERROR, "Unexpected end.", loc());
        }
        void throw_ended(const std::string &reason) {
            if (ended()) throw messages::message_t(message_t::ERROR, "Unexpected end: " + reason, loc());
        }
    public:
        size_t i;

        bool submit() {
            res_i = i;
            return true;
        }

        bool ended() {
            return i == ctx.tokens.size();
        }

        token_t &curr() {
            throw_ended();
            return ctx.tokens[i];
        }

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

        location_t res_loc() {
            if (res_i >= ctx.tokens.size()) return loc();
            else return ctx.tokens[res_i].location.intersect(loc());
        }

        bool parse(const parser_t &parser, data::map_t &out) {
            return parser(ctx, i, out);
        }
        bool try_parse(const parser_t &parser, data::map_t &out, bool silent = true) {
            try {
                return parser(ctx, i, out);
            }
            catch (messages::message_t msg) {
                if (!silent) ctx.messages.push(msg);
                return false;
            }
        }
        bool try_parse(const parser_t &parser, data::map_t &out, message_t &err) {
            try {
                return parser(ctx, i, out);
            }
            catch (messages::message_t msg) {
                err = msg;
                return false;
            }
        }

        bool try_advance() {
            if (ended()) return false;
            i++;
            return !ended();
        }
        void advance() {
            throw_ended();
            i++;
            throw_ended();
        }
        void advance(const std::string &reason) {
            throw_ended(reason);
            i++;
            throw_ended(reason);
        }

        tree_helper_t(ast_ctx_t &ctx, size_t &i): ctx(ctx), res_i(i) {
            this->i = i;
        }
    };
}