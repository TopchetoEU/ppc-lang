#pragma once

#include <vector>
#include <string>

namespace ppc::str {
    std::vector<std::string> split(std::string const &splittable, std::initializer_list<char> splitters, bool remove_empty_entries = false);
    std::string trim(std::string splittable, std::initializer_list<char> splitters = { ' ', '\n', '\t', '\r' });
    inline bool begins_with(std::string const &str, std::string const &other) {
        return !str.find(other);
    }
    inline bool ends_with(std::string const &str, std::string const &other) {
        return str.find_last_of(other) == (str.length() - other.length());
    }
}