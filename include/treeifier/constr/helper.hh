#include "treeifier/constr.hh"

using namespace ppc;
using namespace ppc::lang;
using namespace ppc::data;
using namespace ppc::tree;
using namespace ppc::tree::constr;

namespace ppc::tree::constr {
    struct parse_helper_t {
    private:
        ast_ctx_t &ctx;
        size_t &res_i;

    public:
        size_t i;

        void throw_ended() {
            if (ended()) throw messages::message_t(message_t::ERROR, "Unexpected end.", loc());
        }
        void throw_ended(const std::string &reason) {
            if (ended()) throw messages::message_t(message_t::ERROR, "Unexpected end: " + reason, loc());
        }

        location_t loc(size_t n) {
            location_t res = prev_loc();
            res.start += res.length;
            res.code_start += res.length;
            res.length = n;
            return res;
        }
        location_t prev_loc() {
            auto prev_i = i;
            if (i > 0) i--;
            auto res  = loc();
            i = prev_i;
            return res;
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

        bool err(std::string message) {
            throw message_t::error(message, loc());
        }
        bool err(std::string message, size_t n) {
            throw message_t::error(message, loc(n));
        }

        bool submit(bool inc_i = true) {
            res_i = (i += inc_i);
            return true;
        }

        bool ended() {
            return i == ctx.tokens.size();
        }

        token_t &curr(const std::string &reason) {
            throw_ended(reason);
            return ctx.tokens[i];
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

        template <class T>
        bool parse(const parser_t<T> &parser, T &out) {
            return ctx.parse(parser, i, out);
        }

        template <class T>
        void force_parse(const parser_t<T> &parser, std::string message, T &out) {
            throw_ended(message);
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

        parse_helper_t(ast_ctx_t &ctx, size_t &i): ctx(ctx), res_i(i) {
            this->i = i;
        }
    };
}