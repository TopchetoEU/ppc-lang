#include "utils/location.hh"
#include <sstream>

using namespace ppc;
using namespace std::string_literals;

std::string location_t::to_string() const {
    std::stringstream res;
    bool written_anything = false;

    if (filename.length()) {
        res << filename;
        written_anything = true;
    }
    if (line + 1 != 0) {
        if (written_anything) res << ':';
        res << line + 1;
        written_anything = true;
    }
    if (start + 1 != 0) {
        if (written_anything) res << ':';
        res << start + 1;
        written_anything = true;
    }
    if (length + 1 != 0) {
        if (written_anything) res << '(' << length << ')';
        written_anything = true;
    }

    return res.str();
}
static void fix_location(location_t &loc) {
    if (loc.line == -1u) loc.line = 0;
    if (loc.start == -1u) loc.start = 0;
    if (loc.length == -1u) loc.length = 0;
    if (loc.code_start == -1u) loc.code_start = 0;
}
location_t location_t::intersect(location_t other) const {
    location_t a = *this;
    location_t b = other;

    if (a.start == -1u || b.start == -1u) return { };

    if (a.start > b.start) return other.intersect(*this);

    fix_location(a);
    fix_location(b);

    int a_end = a.code_start + a.length;
    int b_end = b.code_start + b.length;

    if (a_end < b_end) {
        a.length += b_end - a_end;
    }

    return a;
}

bool location_t::operator==(const location_t &other) const {
    if (this->filename != other.filename) return false;
    if (this->line != other.line) return false;
    if (this->start != other.start) return false;
    if (this->length != other.length) return false;
    if (this->code_start != other.code_start) return false;

    return true;
}



std::string empty = "";

location_t::location_t():
    location_t(empty, -1, -1, -1, -1) { }
location_t::location_t(const std::string &filename):
    location_t(filename, -1, -1, -1, -1) { }
location_t::location_t(std::size_t line, std::size_t start):
    location_t(empty, line, start, -1, -1) { }
location_t::location_t(const std::string &filename, std::size_t line, std::size_t start):
    location_t(filename, line, start, -1, -1) { }
location_t::location_t(std::size_t line, std::size_t start, std::size_t code_start):
    location_t(empty, line, start, code_start, -1) { }
location_t::location_t(const std::string &filename, std::size_t line, std::size_t start, std::size_t code_start):
    location_t(filename, line, start, code_start, -1) { }
location_t::location_t(std::size_t line, std::size_t start, std::size_t code_start, std::size_t length):
    location_t(empty, line, start, code_start, length) { }
location_t::location_t(const std::string &filename, std::size_t line, std::size_t start, std::size_t code_start, std::size_t length): filename(filename) {
    this->length = length;
    this->code_start = code_start;
    this->line = line;
    this->start = start;
}

const location_t location_t::NONE = { };
