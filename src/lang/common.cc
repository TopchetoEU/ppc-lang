
#include <sstream>
#include "lang/common.hh"

namespace ppc::lang {
    std::string loc_namespace_name_t::to_string() const {
        std::stringstream res;

        for (size_t i = 0; i < size(); i++) {
            if (i != 0) res << "::";
            res << (*this)[i];
        }

        return res.str();
    }
    std::string namespace_name_t::to_string() const {
        std::stringstream res;

        for (size_t i = 0; i < size(); i++) {
            if (i != 0) res << "::";
            res << (*this)[i];
        }

        return res.str();
    }
    
    bool loc_namespace_name_t::operator==(const loc_namespace_name_t &other) const {
        if (other.size() != size()) return false;

        for (size_t i = 0; i < size(); i++) {
            if (other[i] != (*this)[i]) return false;
        }

        return true;
    }
    bool loc_namespace_name_t::operator!=(const loc_namespace_name_t &other) const {
        if (other.size() != size()) return true;

        for (size_t i = 0; i < size(); i++) {
            if (other[i] == (*this)[i]) return false;
        }

        return true;
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

    namespace_name_t loc_namespace_name_t::strip_location() {
        namespace_name_t res;

        for (const auto &el : *this) {
            res.push_back(el);
        }

        return res;
    }

}

