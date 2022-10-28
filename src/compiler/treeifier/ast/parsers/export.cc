#include "compiler/treeifier/ast/helper.hh"

bool ast::parse_export(ast_ctx_t &ctx, size_t &res_i, map_t &out) {
    tree_helper_t h(ctx, res_i);

    if (out["exported"].is_true()) {
        ctx.messages.push(message_t(message_t::WARNING, "Export is alredy specified for this definition.", h.prev_loc()));
    }
    out["exported"] = true;

    return ctx.group("$_def")(ctx, res_i, out);
}
