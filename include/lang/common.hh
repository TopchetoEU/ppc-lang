#pragma once

#include "utils/message.hh"
#include "utils/location.hh"

namespace ppc::lang {
    struct namespace_name_t {
        std::vector<std::string> segments;
        ppc::location_t location;

        bool operator ==(namespace_name_t const &other);
    };

    bool is_identifier_valid(messages::msg_stack_t &msg_stack, ppc::location_t location, std::string const &name);
    inline bool is_identifier_valid(std::string const &name) {
        messages::msg_stack_t ms { };
        return is_identifier_valid(ms, { }, name);
    }
}
