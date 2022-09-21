#pragma once

#include <vector>
#include <string>

namespace ppc::str {
    std::vector<std::string> split(const std::string &splittable, std::initializer_list<char> splitters, bool remove_empty_entries = false);
    std::string trim(std::string splittable, std::initializer_list<char> splitters = { ' ', '\n', '\t', '\r' });
    inline bool begins_with(const std::string &str, const std::string &other) {
        return !str.find(other);
    }
    inline bool ends_with(const std::string &str, const std::string &other) {
        return str.find_last_of(other) == (str.length() - other.length());
    }
}