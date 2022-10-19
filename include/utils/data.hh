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

        array_t &array(const array_t &arr);
        map_t &map(const map_t &map);
        number_t &number(number_t num);
        string_t &string(const string_t &str);
        bool_t &boolean(bool_t bl);

        array_t &array();
        map_t &map();
        number_t &number();
        string_t &string();
        bool_t &boolean();

        const array_t &array() const;
        const map_t &map() const;
        number_t number() const;
        const string_t &string() const;
        bool_t boolean() const;

        value_t &operator=(const value_t &other);
        value_t &operator=(const char *other);

        ~value_t();
        value_t();
        value_t(const array_t &val);
        value_t(const map_t &val);
        value_t(std::initializer_list<std::pair<std::string, value_t>> map);
        value_t(number_t val);
        value_t(const string_t &val);
        value_t(const char *val);
        value_t(bool_t val);
        value_t(const value_t &other);

    };


    class map_t {
    private:
        std::unordered_map<std::string, value_t> values;
    public:
        value_t &operator[](std::string name){
            auto res = values.find(name);
            if (res == values.end()) {
                res = values.emplace(name, value_t()).first;
            }
            return res->second;
        }
        const value_t &operator [](std::string name) const {
            auto res = values.find(name);
            if (res == values.end()) throw "The map doesn't contain a key '" + name + "'.";
            return res->second;
        }

        bool has(std::string key) const {
            return values.find(key) != values.end();
        }

        std::size_t size() const { return values.size(); }

        auto begin() const { return values.begin(); }
        auto end() const { return values.end(); }

        map_t() { }
        map_t(std::initializer_list<std::pair<std::string, value_t>> vals) {
            for (const auto &pair : vals) {
                values.insert(pair);
            }
        }
    };

    class array_t : public std::vector<value_t> { };
}