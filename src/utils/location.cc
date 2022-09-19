#include "utils/location.hh"
#include <sstream>

using namespace ppc;

std::string location_t::to_string() const {
    std::stringstream res;
    bool written_anything = false;

    if (filename.length()) {
        res << filename;
        written_anything = true;
    }
    if (line != -1u) {
        if (written_anything) res << ':';
        res << line + 1;
        written_anything = true;
    }
    if (start != -1u) {
        if (written_anything) res << ':';
        res << start + 1;
        written_anything = true;
    }
    if (length != -1u) {
        if (written_anything) res << '(' << length + 1 << ')';
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

    if (a.start > b.start) {
        location_t c = a;
        a = b;
        b = c;
    }

    fix_location(a);
    fix_location(b);

    int a_end = a.code_start + a.length;
    int b_end = b.code_start + b.length;

    if (a_end < b_end) {
        a.length += b_end - a_end;
    }

    return a;
}

location_t::location_t() {
    this->line = -1;
    this->start = -1;
    this->length = -1;
    this->code_start = -1;
    this->filename = "";
}
location_t::location_t(std::string filename) {
    this->line = -1;
    this->start = -1;
    this->length = -1;
    this->code_start = -1;
    this->filename = filename;
}
location_t::location_t(std::size_t line, std::size_t start) {
    this->line = line;
    this->start = start;
    this->length = -1;
    this->code_start = -1;
    this->filename = "";
}
location_t::location_t(std::string filename, std::size_t line, std::size_t start) {
    this->line = line;
    this->start = start;
    this->length = -1;
    this->code_start = -1;
    this->filename = filename;
}
location_t::location_t(std::size_t line, std::size_t start, std::size_t code_start) {
    this->line = line;
    this->start = start;
    this->length = -1;
    this->code_start = code_start;
    this->filename = "";
}
location_t::location_t(std::string filename, std::size_t line, std::size_t start, std::size_t code_start) {
    this->line = line;
    this->start = start;
    this->length = -1;
    this->code_start = code_start;
    this->filename = filename;
}
location_t::location_t(std::size_t line, std::size_t start, std::size_t code_start, std::size_t length) {
    this->line = line;
    this->start = start;
    this->length = line;
    this->code_start = code_start;
    this->filename = "";
}
location_t::location_t(std::string filename, std::size_t line, std::size_t start, std::size_t code_start, std::size_t length) {
    this->line = line;
    this->start = start;
    this->length = line;
    this->code_start = code_start;
    this->filename = filename;
}
