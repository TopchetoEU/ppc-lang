#pragma once

#include "utils/message.hh"
#include "utils/location.hh"

namespace ppc::lang {
    struct namespace_name_t {
        std::vector<std::string> segments;
        ppc::location_t location;

        bool operator ==(const namespace_name_t &other);
    };

    bool is_identifier_valid(messages::msg_stack_t &msg_stack, ppc::location_t location, const std::string &name);
    inline bool is_identifier_valid(const std::string &name) {
        messages::msg_stack_t ms { };
        return is_identifier_valid(ms, { }, name);
    }
}
