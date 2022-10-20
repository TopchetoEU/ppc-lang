#pragma once

#include <string>

namespace ppc {
    struct location_t {
        static const location_t NONE;

        std::size_t line;
        std::size_t start;
        std::size_t length;
        std::size_t code_start;
        std::string filename;

        location_t &operator=(const location_t &other);
        bool operator==(const location_t &other) const;
        bool operator !=(const location_t &other) const {
            return !(*this == other);
        }

        operator std::string() const { return to_string(); }
        std::string to_string() const;
        location_t intersect(location_t other) const;

        location_t();
        location_t(const location_t &other): location_t(other.filename, other.line, other.start, other.code_start, other.length) { }
        location_t(const std::string &filename);
        location_t(std::size_t line, std::size_t start);
        location_t(const std::string &filename, std::size_t line, std::size_t start);
        location_t(std::size_t line, std::size_t start, std::size_t code_start);
        location_t(const std::string &filename, std::size_t line, std::size_t start, std::size_t code_start);
        location_t(std::size_t line, std::size_t start, std::size_t code_start, std::size_t length);
        location_t(const std::string &filename, std::size_t line, std::size_t start, std::size_t code_start, std::size_t length);
    };
}