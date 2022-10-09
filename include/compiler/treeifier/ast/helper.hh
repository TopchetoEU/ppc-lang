#include "compiler/treeifier/ast.hh"

using namespace ppc;
using namespace ppc::lang;
using namespace ppc::data;
using namespace ppc::comp::tree;
using namespace ppc::comp::tree::ast;

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

        void err(std::string message) {
            throw message_t::error(message, loc());
        }

        bool submit(bool inc_i = true) {
            res_i = (i += inc_i);
            return true;
        }

        bool ended() {
            return i == ctx.tokens.size();
        }

        token_t &curr() {
            throw_ended();
            return ctx.tokens[i];
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

        bool push_parse(const parser_t &parser, data::array_t &out) {
            data::map_t res;
            if (parser(ctx, i, res)) {
                out.push(res);
                return true;
            }
            else return false;
        }

        bool parse(const parser_t &parser, data::map_t &out) {
            return parser(ctx, i, out);
        }

        void force_push_parse(const parser_t &parser, std::string message, data::array_t &out) {
            advance(message);
            bool success;

            try {
                success = push_parse(parser, out);
            }
            catch (const message_t &msg) {
                ctx.messages.push(msg);
                success = false;
            }
            
            if (!success) err(message);
        }
        void force_parse(const parser_t &parser, std::string message, data::map_t &out) {
            advance(message);
            bool success;

            try {
                success = parse(parser, out);
            }
            catch (const message_t &msg) {
                ctx.messages.push(msg);
                success = false;
            }
            
            if (!success) err(message);
        }

        tree_helper_t(ast_ctx_t &ctx, size_t &i): ctx(ctx), res_i(i) {
            this->i = i;
        }
    };
}