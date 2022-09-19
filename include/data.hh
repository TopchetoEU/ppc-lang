#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace ppc::data {
    class map_t;
    class array_t;
    using number_t = float;
    using string_t = std::string;
    using bool_t = bool;

    class value_t {
    private:
        // Variant and the C++ comittee can go fuck themselves
        union val_t {
            map_t *map;
            array_t *arr;
            number_t num;
            string_t *str;
            bool_t bl;
        } val;

        enum type_t {
            Null,
            Map,
            Arr,
            Num,
            Str,
            Bool,
        } type;
    public:
        bool is_null() const;
        bool is_map() const;
        bool is_array() const;
        bool is_number() const;
        bool is_string() const;
        bool is_bool() const;

        bool array(array_t &out) const;
        bool map(map_t &out) const;
        bool number(number_t &out) const;
        bool string(string_t &out) const;
        bool boolean(bool_t &out) const;

        array_t const &array() const;
        map_t const &map() const;
        number_t number() const;
        string_t const &string() const;
        bool_t boolean() const;

        // value_t &operator=(value_t const &other);

        ~value_t();
        value_t();
        value_t(array_t const &val);
        value_t(map_t const &val);
        value_t(number_t val);
        value_t(string_t const &val);
        value_t(bool_t val);
        value_t(value_t const &other);
    };


    class map_t {
    private:
        std::unordered_map<std::string, value_t> values;
    public:
        value_t &operator [](std::string name) {
            if (values.find(name) == values.end()) {
                values.emplace(name, value_t { });
            }

            return values[name];
        }

        std::size_t size() const { return values.size(); }

        auto begin() const { return values.begin(); }
        auto end() const { return values.end(); }
    };

    class array_t {
    private:
        std::vector<value_t> values;
    public:
        value_t &operator [](std::size_t i) { return values[i]; }

        auto begin() { return values.begin(); }
        auto end() { return values.end(); }

        void push(value_t const &val) { values.push_back(val); }
        void insert(value_t const &val, std::size_t i = 0) { values.insert(begin() + i, val); }
        void pop() { values.pop_back(); }
        void remove(std::size_t i = 0) { values.erase(begin() + i); }

        std::size_t size() const { return values.size(); }
    };
}