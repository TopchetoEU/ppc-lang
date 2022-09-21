#include <iostream>
#include <sstream>
#include "utils/message.hh"

using namespace ppc;

std::string messages::message_t::to_string() const {
    std::string loc_readable = location.to_string();
    std::string level_readable;

    switch (level) {
        case messages::message_t::DEBUG: level_readable = "debug"; break;
        case messages::message_t::SUGGESTION: level_readable = "suggestion"; break;
        case messages::message_t::INFO: level_readable = "info"; break;
        case messages::message_t::WARNING: level_readable = "warning"; break;
        case messages::message_t::ERROR: level_readable = "error"; break;
        default: level_readable = "what?"; break;
    }

    std::stringstream res { };

    if (loc_readable.length()) res << loc_readable << ": ";
    res << level_readable << ": " << content;

    return res.str();
}
bool messages::message_t::is_severe() const {
    return level > messages::message_t::WARNING;
}

bool messages::msg_stack_t::is_failed() const {
    for (const auto &msg : messages) {
        if (msg.is_severe()) return true;
    }
    return false;
}
void messages::msg_stack_t::print(std::ostream &output, messages::message_t::level_t threshold, bool color_output) const {
    if (!messages.size()) return;

    for (const auto &msg : messages) {
        if (msg.level < threshold) continue;

        std::string loc_readable = msg.location.to_string();
        
        switch (msg.level) {
            case messages::message_t::DEBUG:
                output << (color_output ? "\e[38;5;8mdebug: " : "debug: ");
                break;
            case messages::message_t::SUGGESTION:
                output << (color_output ? "\e[38;5;45msuggestion: " : "suggestion: ");
                break;
            case messages::message_t::INFO:
                output << (color_output ? "\e[38;5;33minfo: ": "info: ");
                break;
            case messages::message_t::WARNING:
                output << (color_output ? "\e[38;5;214mwarning: " : "warning: ");
                break;
            case messages::message_t::ERROR:
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
