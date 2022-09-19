#include "lang/version.hh"

using namespace ppc;

bool version_t::operator==(version_t other) const {
    bool major_same = major == other.major;
    bool minor_same = minor == -1 || other.minor == -1 || minor == other.minor;
    bool revision_same = revision == -1 || other.revision == -1 || revision == other.revision;

    return major_same && minor_same && revision_same;
}
bool version_t::is_compliant(version_t other) const {
    bool major_compliant = major == other.major;
    bool minor_compliant = minor == -1 || other.minor == -1 || minor <= other.minor;

    return major_compliant && minor_compliant;
}
