#pragma once

#include "utils/message.hh"
#include "utils/location.hh"

namespace ppc::lang {
    template <class T>
    struct located_t : T {
        location_t location;

        located_t(location_t loc, const T &val): T(val), location(loc) { }
        located_t(const T &val): T(val), location(location_t::NONE) { }
        located_t() { }
    };

    struct namespace_name_t : public std::vector<std::string> {
        using base = std::vector<std::string>;

        bool operator ==(const namespace_name_t &other)  const;
        bool operator !=(const namespace_name_t &other)  const;

        std::string to_string() const;

        namespace_name_t() { }
        namespace_name_t(std::initializer_list<std::string> segments): base(segments.begin(), segments.end()) { }
    };

    bool is_identifier_valid(messages::msg_stack_t &msg_stack, ppc::location_t location, const std::string &name);
    inline bool is_identifier_valid(const std::string &name) {
        messages::msg_stack_t ms { };
        return is_identifier_valid(ms, { }, name);
    }
}
