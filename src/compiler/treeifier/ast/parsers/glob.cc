#include "compiler/treeifier/ast.hh"
#include "compiler/treeifier/ast/helper.hh"

using namespace ppc::comp::tree::ast;

class nmsp_def_parser_t : public parser_t {
    bool parse(ast_ctx_t &ctx, size_t &res_i, data::map_t &res) const {
        tree_helper_t h(ctx, res_i);
        if (h.ended()) return false;

        if (!h.curr().is_identifier("namespace")) return false;
        h.advance("Expected a namespace");
        h.force_parse("$_nmsp", "Expected a namespace.", res);
        if (!h.curr().is_operator(operator_t::SEMICOLON)) {
            ctx.messages.push(message_t::error("Expected a semicolon.", h.loc(1)));
            return h.submit(false);
        }
        return h.submit(true);
    }

    public: nmsp_def_parser_t(): parser_t("$_nmsp_def") { }
};
class import_parser_t : public parser_t {
    bool parse(ast_ctx_t &ctx, size_t &res_i, data::map_t &res) const {
        tree_helper_t h(ctx, res_i);
        if (h.ended()) return false;

        if (!h.curr().is_identifier("import")) return false;
        h.advance("Expected a namespace");
        h.force_parse("$_nmsp", "Expected a namespace.", res);
        if (!h.curr().is_operator(operator_t::SEMICOLON)) {
            ctx.messages.push(message_t::error("Expected a semicolon.", h.loc(1)));
            return h.submit(false);
        }

        return h.submit(true);
    }

    public: import_parser_t(): parser_t("$_import") { }
};

auto import_parser = import_parser_t();
auto nmsp_def_parser = nmsp_def_parser_t();

class glob_parser_t : public parser_t {
    bool parse(ast_ctx_t &ctx, size_t &res_i, data::map_t &out) const {
        tree_helper_t h(ctx, res_i);

        return h.parse("$_exp", out);

        if (h.ended()) return true;
        if (nmsp_def_parser(ctx, h.i, (out["namespace"] = map_t()).map())) {
            ctx.nmsp = conv::map_to_nmsp(out["namespace"].map());
        }

        auto &imports = (out["imports"] = array_t()).array();
        auto &contents = (out["content"] = array_t()).array();

        while (true) {
            map_t map;
            if (!import_parser(ctx, h.i, map)) break;
            imports.push_back(map);
            auto nmsp = conv::map_to_nmsp(map);

            if (!ctx.imports.emplace(nmsp).second) h.err("The namespace '" + nmsp.to_string() + "' is already imported.");
        }

        while (true) {
            if (h.ended()) break;
            if (!h.push_parse("$_def", contents)) {
                ctx.messages.push(message_t::error("Invalid token.", h.loc()));
                h.i++;
            }
        }

        if (!h.ended()) h.err("Invalid token.");

        return h.submit();
    }

public:
    glob_parser_t(): parser_t("$_glob") { }
};

const parser_adder_t ppc::comp::tree::ast::glob_adder = [](ast_ctx_t &ctx) {
    ctx.add_group("$_def");
    ctx.add_group("$_exp_val");
    ctx.add_parser(new glob_parser_t());
};
