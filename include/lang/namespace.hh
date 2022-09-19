#pragma once

#include "lang/common.hh"
#include "lang/types.hh"
#include "lang/version.hh"

namespace ppc::lang {
    // A structure, containing all the definitions of a namespace
    struct namespace_t {
        // The name of the namespace
        namespace_name_t name;
        // The version of the namespace
        version_t version;
        // A list of all the defined types inside the namespace
        std::vector<ppc::lang::type_def_t> types;
        // A list of all the defined fields inside the namespace
        std::vector<field_t> fields;

        bool contains_def(std::string const &name, location_t &ploc) const;
        inline bool contains_def(std::string const &name) const {
            location_t ploc;
            return contains_def(name, ploc);
        }

        template <class T>
        static bool contains_def(T namespaces, namespace_name_t const &def_nmsp, std::string const &name, location_t &ploc) {
            for (namespace_t const &nmsp : namespaces) {
                if (nmsp.name == def_nmsp && nmsp.contains_def(name)) return true;
            }
            return false;
        }
        template <class T>
        static inline bool contains_def(T namespaces, namespace_name_t const &def_nmsp, std::string const &name) {
            location_t ploc;
            return contains_def(namespaces, def_nmsp, name, ploc);
        }
    };
}
