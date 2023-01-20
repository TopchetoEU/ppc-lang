#include "lang/common.hh"
#include <sstream>

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

    int namespace_name_t::compare(const namespace_name_t &b) const {
        const auto &a = *this;
        for (size_t i = 0; i < a.size() && i < b.size(); i++) {
            auto cmp = a[i].compare(b[i]);
            if (cmp != 0) return cmp;
        }

        if (a.size() > b.size()) return 1;
        else if (a.size() == b.size()) return 0;
        else return -1;
    }
    int loc_namespace_name_t::compare(const loc_namespace_name_t &b) const {
        const auto &a = *this;
        for (size_t i = 0; i < a.size() && i < b.size(); i++) {
            auto cmp = a[i].compare(b[i]);
            if (cmp != 0) return cmp;
        }

        if (a.size() > b.size()) return 1;
        else if (a.size() == b.size()) return 0;
        else return -1;
    }

    namespace_name_t loc_namespace_name_t::strip_location() const {
        namespace_name_t res;

        for (const auto &el : *this) {
            res.push_back(el);
        }

        return res;
    }
}
