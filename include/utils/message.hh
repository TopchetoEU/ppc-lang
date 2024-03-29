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
        std::string content;
        location_t location;

        message_t(level_t level, const std::string &content, location_t loc = location_t::NONE) :
            level(level),
            content(content),
            location(loc) { }
        message_t() : message_t(DEBUG, "") { }

        operator std::string() const { return to_string(); }

        std::string to_string() const;
        bool is_severe() const;

        static message_t error(const std::string &message, location_t loc = location_t::NONE) { return message_t(ERROR, message, loc); }
    };

    struct msg_stack_t {
    private:
        std::vector<message_t> messages;
    public:
        inline auto begin() { return messages.begin(); }
        inline auto end() { return messages.end(); }

        inline auto begin() const { return messages.begin(); }
        inline auto end() const { return messages.end(); }

        void push(const message_t &msg) { messages.push_back(msg); }
        void err(const std::string &msg, location_t loc = location_t::NONE) { push({ message_t::ERROR, msg, loc }); }
        void warn(const std::string &msg, location_t loc = location_t::NONE) { push({ message_t::WARNING, msg, loc }); }
        void suggest(const std::string &msg, location_t loc = location_t::NONE) { push({ message_t::SUGGESTION, msg, loc }); }
        void info(const std::string &msg, location_t loc = location_t::NONE) { push({ message_t::INFO, msg, loc }); }
        void debug(const std::string &msg, location_t loc = location_t::NONE) { push({ message_t::DEBUG, msg, loc }); }
        void push(const msg_stack_t &other) {
            for (const auto &msg : other) push(msg);
        }
        const message_t &peek() { return messages.back(); }
        void clear() { messages.clear(); }

        bool is_failed() const;
        void print(std::ostream &output, messages::message_t::level_t threshold = messages::message_t::DEBUG, bool color_output = false) const;
    };
}
