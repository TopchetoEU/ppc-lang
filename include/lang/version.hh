#pragma once

#include <cstdint>

namespace ppc {
    // A structure, representing all versions, used troughout the ++C language. 64 bits
    struct version_t {
        /*  The major component of the version
            Used to indicate breaking changes.

            If -1 (65535), then is ignored

            Must match target version in order to be usable */
        std::uint16_t major;
        /*  The minor component of the version
            Used to indicate non-breaking changes (added features).

            If -1 (65535), then is ignored

            Target version's minor version must be bigger or equals to this in order to be usable */
        std::uint16_t minor;
        /*  The revision component of the version
            Used to indicate minor fixes and changes that don't affect the API in any capacity

            If -1 (4 294 967 295), then is ignored

            Ignored when testing for compliancy */
        std::uint32_t revision;

        bool is_compliant(version_t dependency) const;

        // Checks whether or not two versions equal each other (ignoring -1 components)
        bool operator ==(version_t other) const;
        inline bool operator !=(version_t other) const { return !(*this == other); }

        version_t(uint16_t major, uint16_t minor, uint32_t revision) : major { major }, minor { minor }, revision { revision } { }
        version_t(uint16_t major, uint16_t minor) : version_t { major, minor, -1u } { }
        version_t(uint16_t major) : version_t { major, -1u, -1u } { }
    };
}
