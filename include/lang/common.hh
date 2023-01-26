#pragma once

#include <vector>
#include <string>

namespace ppc::lang {
    struct nmsp_t: public std::vector<std::string> {
        using base = std::vector<std::string>;

        int compare(const nmsp_t &other) const;

        bool operator==(const nmsp_t &other) const { return compare(other) == 0; }
        bool operator!=(const nmsp_t &other) const { return compare(other) != 0; }
        bool operator<(const nmsp_t &other) const { return compare(other) < 0; }
        bool operator<=(const nmsp_t &other) const { return compare(other) <= 0; }
        bool operator>(const nmsp_t &other) const  { return compare(other) > 0; }
        bool operator>=(const nmsp_t &other) const { return compare(other) >= 0; }

        operator std::string() const { return to_string(); }
        std::string to_string() const;

        nmsp_t() { }
        nmsp_t(std::initializer_list<std::string> segments): base(segments.begin(), segments.end()) { }
    };
}

template <>
struct std::hash<ppc::lang::nmsp_t> {
    std::size_t operator()(const ppc::lang::nmsp_t& k) const {
        size_t res = 0;

        for (auto &el : k) {
            res ^= std::hash<std::string>()(el);
        }

        return res;
    }
};

#include "utils/message.hh"
#include "utils/location.hh"

namespace ppc::lang {
    template <class ParserT>
    struct located_t: ParserT {
        location_t location;

        located_t(location_t loc, const ParserT &val): ParserT(val), location(loc) { }
        located_t(const ParserT &val): ParserT(val), location(location_t::NONE) { }
        located_t() { }
    };

    struct loc_nmsp_t: public std::vector<located_t<std::string>> {
        using base = std::vector<located_t<std::string>>;

        int compare(const loc_nmsp_t &other) const;

        bool operator==(const loc_nmsp_t &other) const { return compare(other) == 0; }
        bool operator!=(const loc_nmsp_t &other) const { return compare(other) != 0; }
        bool operator<(const loc_nmsp_t &other) const { return compare(other) < 0; }
        bool operator<=(const loc_nmsp_t &other) const { return compare(other) <= 0; }
        bool operator>(const loc_nmsp_t &other) const  { return compare(other) > 0; }
        bool operator>=(const loc_nmsp_t &other) const { return compare(other) >= 0; }

        nmsp_t strip_location() const;

        operator nmsp_t() { return strip_location(); }
        operator std::string() const { return to_string(); }
        std::string to_string() const;

        loc_nmsp_t() { }
        loc_nmsp_t(std::initializer_list<located_t<std::string>> segments): base(segments.begin(), segments.end()) { }
    };

    template <class SetT>
    bool resolve_name(const SetT &defs, const nmsp_t &src, const nmsp_t &target) {
        if (src == target) return true;

        for (auto &it : defs) {
            nmsp_t val = (nmsp_t)it;
            val.insert(val.end(), src.begin(), src.end());

            if (val == target) return true;
        }
    }
}