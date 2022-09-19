#pragma once

#include "utils/location.hh"
#include "lang/common.hh"
#include "lang/version.hh"

namespace ppc::lang {
    enum def_type_kind_t {
        TYPE_NONE,
        TYPE_STRUCT,
    };

    struct type_def_t;

    // A structure, representing a ++C type notation
    struct type_t {
        // The type definition of which this type is
        // If NULL, then this type is unknown
        // Feel free to scream at the user if this is NULL
        type_def_t *def;
    };
    // A structure, representing a field in a ++C struct
    struct field_t {
        // The type of which this field is
        type_t type;
        // The name of the field
        const char *name;
        // Whether or not the field is exported
        bool is_exported;
    };

    // A structure, representing a ++C type (structs, classes, delegates and interfaces)
    struct type_def_t {
        // Whether or not this definition is exported
        bool is_exported;
        // The namespace of the type
        namespace_name_t _namespace;
        // The name of the type
        const char *name;
        // The version of the type (inherited from the module version)
        version_t version;
        // The location of the definition
        location_t location;

        // The alignment padding from the start of the structures, in bytes
        // Used either for efficiency sake or to store private fields (or both)
        std::size_t align_size;
        // A list of all the type's fields
        std::vector<field_t> fields;
    };
}
