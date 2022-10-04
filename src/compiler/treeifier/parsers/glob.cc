#include "compiler/treeifier/ast.hh"
#include "compiler/treeifier/ast/helper.hh"

namespace ppc::comp::tree::ast {
    class glob_parser_t : public parser_t {
        bool parse_nmsp(ast_ctx_t &ctx, size_t &res_i, located_t<namespace_name_t> &out) const {
            tree_helper_t h(ctx, res_i);
            located_t<namespace_name_t> res;

            while (true) {
                auto &curr = h.curr();

                if (h.ended() || !curr.is_identifier()) return false;
                else res.push_back(curr.identifier());

                if (!h.try_advance() || !h.curr().is_operator(operator_t::DOUBLE_COLON)) {
                    out = res;
                    return h.submit();
                }
            }
        }
        bool parse_nmsp_def(ast_ctx_t &ctx, size_t &res_i) const {
            tree_helper_t h(ctx, res_i);
            if (h.ended()) return true;

            if (h.curr().is_identifier("namespace")) {
                h.advance("Expected a namespace name.");
                if (!parse_nmsp(ctx, h.i, ctx.nmsp)) throw message_t::error("Expected a namespace name.", h.loc());
                return h.submit();
            }
            else return false;
        }
        bool parse_import(ast_ctx_t &ctx, size_t &res_i) const {
            tree_helper_t h(ctx, res_i);
            if (h.ended()) return true;

            if (h.curr().is_identifier("import")) {
                h.advance("Expected a namespace name.");
                located_t<namespace_name_t> name;
                if (!parse_nmsp(ctx, h.i, name)) throw message_t::error("Expected a namespace name.", h.loc());
                if (!ctx.imports.emplace(name).second) {
                    throw message_t::error("The namespace '" + name.to_string() + "' is already imported.", h.loc());
                }
                return h.submit();
            }
            else return false;
        }

        bool parse(ast_ctx_t &ctx, size_t &res_i, data::map_t &out) const {
            tree_helper_t h(ctx, res_i);
            if (h.ended()) return true;
            parse_nmsp_def(ctx, h.i);

            while (parse_import(ctx, h.i));

            return true;
        }
    };

    const parser_t &glob_parser = glob_parser_t();
}
