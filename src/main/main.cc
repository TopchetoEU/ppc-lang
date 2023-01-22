#ifdef WINDOWS

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x4
#endif

#ifndef DISABLE_NEWLINE_AUTO_RETURN
#define DISABLE_NEWLINE_AUTO_RETURN 0x8
#endif

#include <conio.h>
#include <windows.h>

#undef ERROR
#undef INFO

#endif

#include "./opions.hh"
#include "treeifier/constr.hh"
#include "treeifier/constr/glob.hh"
#include "treeifier/lexer.hh"
#include "treeifier/tokenizer.hh"
#include "utils/json.hh"
#include "utils/strings.hh"
#include "utils/threading.hh"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

using std::cout;
using std::size_t;
using namespace ppc;
using namespace ppc::tree;
using namespace ppc::tree::constr;

void add_flags(options::parser_t &parser) {
    parser.add_flag({
        .name = "version",
        .shorthands = "v",
        .description = "Displays version and license agreement of this binary",
        .execute = [](options::parser_t &parser, const std::string &option, ppc::messages::msg_stack_t &global_stack) {
            cout << "++C compiler\n";
            cout << "    Version: v" << PPC_VERSION_MAJOR << '.' << PPC_VERSION_MINOR << '.' << PPC_VERSION_BUILD;
#if WINDOWS
            cout << " (Windows)";
#elif LINUX
            cout << " (Linux)";
#endif
            cout << "\n";
            cout << "    License: MIT Copyright (C) TopchetoEU\n";
            exit(0);
        }
    });
    parser.add_flag({
        .name = "help",
        .shorthands = "h",
        .description = "Displays a list of all flags and their meaning",
        .execute = [](options::parser_t &parser, const std::string &option, ppc::messages::msg_stack_t &global_stack) {
            cout << "Usage: ...flags ...files\n\n";
            cout << "Flags and file names can be interlaced\n";
            cout << "Flags will execute in the order they're written, then compilation begins\n\n";
            cout << "Flags:\n";

            for (const auto &flag : parser) {
                std::stringstream buff;
                buff << "    --" << flag.name;

                if (flag.match_type) buff << "=...";
                if (flag.shorthands.size()) {
                    buff << " (";
                    bool first = true;
                    for (char shorthand : flag.shorthands) {
                        if (!first) buff << ",";
                        else first = false;

                        buff << " -";
                        buff << std::string { shorthand };
                    }
                    buff << ")";
                }

                buff << " ";

                cout << buff.str();
                size_t n = buff.str().length();

                if (flag.description.size()) {
                    const size_t padding = 24;
                    const size_t msg_width = 80 - padding;

                    for (size_t i = 0; i < padding - n; i++)
                        cout << ' ';

                    int len = flag.description.length();

                    for (size_t i = 0; i < len / msg_width; i++) {
                        for (size_t j = 0; j < msg_width; j++)
                            cout << flag.description[i * msg_width + j];
                        cout << std::endl;
                        for (size_t j = 0; j < padding; j++)
                            cout << ' ';
                    }

                    int remainder = len % msg_width;

                    for (int i = 0; i < remainder; i++)
                        cout << flag.description[len - remainder + i];
                }

                printf("\n");
            }
        }
    });
    parser.add_flag({
        .name = "silent",
        .shorthands = "qs",
        .description = "Doesn't display any messages, regardless of their severity",
    });
    parser.add_flag({
        .name = "msg-threshold",
        .description = "Sets a lower limit of messages that will print. Accepted values: 'all', 'debug', 'suggestion', 'info', 'warning', 'error', 'none'",
        .match_type = options::MATCH_PREFIX,
    });
    parser.add_flag({
        .name = "print-what",
        .description = "Prints a 'what?' type of message (you'll see)",
        .match_type = options::MATCH_PREFIX,
        .execute = [](options::parser_t &parser, const std::string &option, ppc::messages::msg_stack_t &global_stack) {
            global_stack.push(messages::message_t((messages::message_t::level_t)69, "IDK LOL."));
        }
    });
}

int main(int argc, const char *argv[]) {
#ifdef WINDOWS
    HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(handleOut, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    consoleMode |= DISABLE_NEWLINE_AUTO_RETURN;
    SetConsoleMode(handleOut, consoleMode);
#endif

    std::vector<std::string> args { argv + 1, argv + argc };
    std::vector<std::string> files;
    messages::msg_stack_t msg_stack;

    try {
        options::parser_t parser;
        data::map_t conf;
        add_flags(parser);

        for (const auto &arg : args) {
            if (!parser.parse(arg, msg_stack, conf)) {
                files.push_back(arg);
            }
        }

        for (const auto &file : files) {
            try {
                std::ifstream f { file, std::ios_base::in };
                if (!f.is_open()) throw message_t::error("The file doesn't exist.", { file });
                auto tokens = token_t::parse_many(msg_stack, lex::token_t::parse_file(msg_stack, file, f));
                auto ast = ast_ctx_t::parse(constr::glob_parser_t(), msg_stack, tokens);
                // std::cout << data::json::stringify(ast) << std::endl;
            }
            catch (const messages::message_t &msg) {
                msg_stack.push(msg);
            }
        }
    }
    catch (const messages::message_t &msg) {
        msg_stack.push(msg);
    }
#ifndef PROFILE_debug
    catch (const std::string &msg) {
        msg_stack.push(message_t::error(msg));
    }
    catch (...) {
        std::cout << std::endl;
        msg_stack.push(message_t::error("A fatal error occurred."));
    }
#endif

    msg_stack.print(std::cout, messages::message_t::DEBUG, true);

    return 0;
}
