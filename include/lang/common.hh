#pragma once

#include "utils/message.hh"
#include "utils/location.hh"

namespace ppc::lang {
    template <class T>
    struct located_t : T {
        location_t location;

        template <class ...Args>
        located_t(location_t loc, Args ...args): T(args...), location(loc) { }
        template <class ...Args>
        located_t(Args ...args): T(args...), location(location_t::NONE) { }
    };

    struct namespace_name_t {
        std::vector<std::string> segments;

        bool is_empty() const { return segments.empty(); }

        auto begin() { return segments.begin(); }
        auto end() { return segments.end(); }

        bool operator ==(const namespace_name_t &other)  const;
        const std::string &operator[](size_t i) const { return segments[i]; }

        std::string to_string() const;

        namespace_name_t() { }
        namespace_name_t(std::initializer_list<std::string> segments): segments(segments.begin(), segments.end()) { }
    };

    bool is_identifier_valid(messages::msg_stack_t &msg_stack, ppc::location_t location, const std::string &name);
    inline bool is_identifier_valid(const std::string &name) {
        messages::msg_stack_t ms { };
        return is_identifier_valid(ms, { }, name);
    }
}
