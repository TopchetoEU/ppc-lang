
#include <sstream>
#include "lang/common.hh"

namespace ppc::lang {
    std::string namespace_name_t::to_string() const {
        std::stringstream res;

        for (size_t i = 0; i < segments.size(); i++) {
            if (i != 0) res << "::";
            res << segments[i];
        }

        return res.str();
    }

    bool namespace_name_t::operator==(const namespace_name_t &other) const {
        if (other.segments.size() != segments.size()) return false;

        for (size_t i = 0; i < segments.size(); i++) {
            if (other[i] != segments[i]) return false;
        }

        return true;
    }
}

