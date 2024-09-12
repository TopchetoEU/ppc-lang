#include "lang/common.hh"
#include <sstream>

namespace ppc::lang {
    std::string loc_nmsp_t::to_string() const {
        std::stringstream res;

        for (size_t i = 0; i < size(); i++) {
            if (i != 0) res << "::";
            res << (*this)[i];
        }

        return res.str();
    }
    std::string nmsp_t::to_string() const {
        std::stringstream res;

        for (size_t i = 0; i < size(); i++) {
            if (i != 0) res << "::";
            res << (*this)[i];
        }

        return res.str();
    }

    int nmsp_t::compare(const nmsp_t &b) const {
        const auto &a = *this;
        for (size_t i = 0; i < a.size() && i < b.size(); i++) {
            auto cmp = a[i].compare(b[i]);
            if (cmp != 0) return cmp;
        }

        if (a.size() > b.size()) return 1;
        else if (a.size() == b.size()) return 0;
        else return -1;
    }
    int loc_nmsp_t::compare(const loc_nmsp_t &b) const {
        const auto &a = *this;
        for (size_t i = 0; i < a.size() && i < b.size(); i++) {
            auto cmp = a[i].compare(b[i]);
            if (cmp != 0) return cmp;
        }

        if (a.size() > b.size()) return 1;
        else if (a.size() == b.size()) return 0;
        else return -1;
    }

    nmsp_t loc_nmsp_t::strip_location() const {
        nmsp_t res;

        for (const auto &el : *this) {
            res.push_back(el);
        }

        return res;
    }
}
