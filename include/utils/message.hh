#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "utils/location.hh"

namespace ppc::messages {
    struct message_t {
        enum level_t {
            DEBUG,
            SUGGESTION,
            INFO,
            WARNING,
            ERROR,
        } level;
        location_t location;
        std::string content;

        message_t(level_t level, std::string content, location_t loc = location_t::NONE) :
            level(level),
            content(content),
            location(loc) { }
        message_t() : message_t(DEBUG, "") { }

        std::string to_string() const;
        bool is_severe() const;
    };

    struct msg_stack_t {
    private:
        std::vector<message_t> messages;
    public:
        inline auto begin() { return messages.begin(); }
        inline auto end() { return messages.end(); }

        void push(const message_t &msg) { messages.push_back(msg); }
        void clear() { messages.clear(); }

        bool is_failed() const;
        void print(std::ostream &output, messages::message_t::level_t threshold = messages::message_t::DEBUG, bool color_output = false) const;
    };
}
