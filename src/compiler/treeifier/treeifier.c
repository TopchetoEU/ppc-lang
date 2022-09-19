#include "compiler/treeifier.h"
#include "compiler/treeifier/lexer.h"
#include "compiler/treeifier/tokenizer.h"
#include "compiler/treeifier/ast.h"

inline static const char* read_file(const char *path) {
    FILE *file = fopen(path, "rb");

    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    char *data = malloc(char, len + 1);
    data[len] = 0;
    fseek(file, 0, SEEK_SET);
    #pragma GCC diagnostic ignored "-Wunused-result"
    fread(data, 1, len, file);
    fclose(file);

    return data;
}

bool ppc_treeify(msg_stack_t *msg_stack, const char *filename, const char *source, ast_namespace_t *pout) {
    lex_tokens_t lex_tokens = { 0 };
    tokens_t tokens = { 0 };
    ast_namespace_t ast = { 0 };
    if (!ppc_lex(msg_stack, filename, source, &lex_tokens)) goto failed;
    if (!ppc_tokeinze(msg_stack, lex_tokens, &tokens)) goto failed;
    ppc_lex_free(lex_tokens);
    if (!ppc_ast(msg_stack, tokens, &ast)) goto failed;
    ppc_tokeinze_free(tokens);

    if (pout) *pout = ast;
    return true;

failed:
    ppc_lex_free(lex_tokens);
    ppc_tokeinze_free(tokens);
    ast_namespace_free(ast);
    return false;

}
bool ppc_treeify_file(const char *filename, msg_stack_t *pmsg_stack, ast_namespace_t *pout) {
    const char *source = read_file(filename);
    msg_stack_t msg_stack = { 0 };

    if (!source) {
        if (pmsg_stack) {
            const char errmsg[] = "The file '%s' was not found.";
            char *errmsgMem = malloc(char, strlen(filename) + sizeof(errmsg) - 2);
            sprintf(errmsgMem, errmsg, filename);
            free(filename);

            list_add(msg_stack, ((message_t) {
                .content = errmsgMem,
                .level = MSGLEVEL_ERROR,
                .location = loc_from_file(filename),
                .malloced = true,
            }));

            *pmsg_stack = msg_stack;
        }

        goto failed;
    }

    ast_namespace_t ast = { 0 };
    if (!ppc_treeify(&msg_stack, filename, source, &ast)) goto failed;

    free(source);
    if (pout) *pout = ast;
    if (pmsg_stack) *pmsg_stack = msg_stack;
    return true;

failed:
    free(source);
    if (pmsg_stack) *pmsg_stack = msg_stack;
    return false;
}
