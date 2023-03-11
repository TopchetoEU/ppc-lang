#include "utils/message.hh"
#include <iostream>
#include <sstream>

using namespace ppc;

namespace ppc::messages {
    std::string message_t::to_string() const {
        std::string loc_readable = location.to_string();
        std::string level_readable;

        switch (level) {
            case message_t::DEBUG:
                level_readable = "debug";
                break;
            case message_t::SUGGESTION:
                level_readable = "suggestion";
                break;
            case message_t::INFO:
                level_readable = "info";
                break;
            case message_t::WARNING:
                level_readable = "warning";
                break;
            case message_t::ERROR:
                level_readable = "error";
                break;
            default:
                level_readable = "what?";
                break;
        }

        std::stringstream res {};

        if (loc_readable.length()) res << loc_readable << ": ";
        res << level_readable << ": " << content;

        return res.str();
    }
    bool message_t::is_severe() const {
        return level > message_t::WARNING;
    }

    bool msg_stack_t::is_failed() const {
        for (const auto &msg : messages) {
            if (msg.is_severe()) return true;
        }
        return false;
    }
    void msg_stack_t::print(std::ostream &output, message_t::level_t threshold, bool color_output) const {
        if (!messages.size()) return;

        for (const auto &msg : messages) {
            if (msg.level < threshold) continue;

            std::string loc_readable = msg.location.to_string();

            switch (msg.level) {
                case message_t::DEBUG:
                    output << (color_output ? "\e[38;5;8mdebug: " : "debug: ");
                    break;
                case message_t::SUGGESTION:
                    output << (color_output ? "\e[38;5;45msuggestion: " : "suggestion: ");
                    break;
                case message_t::INFO:
                    output << (color_output ? "\e[38;5;33minfo: " : "info: ");
                    break;
                case message_t::WARNING:
                    output << (color_output ? "\e[38;5;214mwarning: " : "warning: ");
                    break;
                case message_t::ERROR:
                    output << (color_output ? "\e[38;5;196merror: " : "error: ");
                    break;
                default:
                    output << (color_output ? "\e[38;5;196mw\e[38;5;226mh\e[38;5;118ma\e[38;5;162mt\e[38;5;129m?\e[0m: " : "what?: ");
                    break;
            }

            if (loc_readable.length()) output << loc_readable << ": ";
            output << msg.content;
            if (color_output) output << "\e[0m";
            output << std::endl;
        }
    }
}