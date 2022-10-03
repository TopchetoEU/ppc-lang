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

        std::string to_string() const;
        location_t intersect(location_t other) const;

        location_t();
        location_t(std::string filename);
        location_t(std::size_t line, std::size_t start);
        location_t(std::string filename, std::size_t line, std::size_t start);
        location_t(std::size_t line, std::size_t start, std::size_t code_start);
        location_t(std::string filename, std::size_t line, std::size_t start, std::size_t code_start);
        location_t(std::size_t line, std::size_t start, std::size_t code_start, std::size_t length);
        location_t(std::string filename, std::size_t line, std::size_t start, std::size_t code_start, std::size_t length);
    };
}