
#include <sstream>
#include "lang/common.hh"

namespace ppc::lang {
    std::string namespace_name_t::to_string() const {
        std::stringstream res;

        for (size_t i = 0; i < size(); i++) {
            if (i != 0) res << "::";
            res << (*this)[i];
        }

        return res.str();
    }

    bool namespace_name_t::operator==(const namespace_name_t &other) const {
        if (other.size() != size()) return false;

        for (size_t i = 0; i < size(); i++) {
            if (other[i] != (*this)[i]) return false;
        }

        return true;
    }
    bool namespace_name_t::operator!=(const namespace_name_t &other) const {
        if (other.size() != size()) return true;

        for (size_t i = 0; i < size(); i++) {
            if (other[i] == (*this)[i]) return false;
        }

        return true;
    }
}

