#include "utils/data.hh"

namespace ppc::data {
    bool value_t::is_null() const {
        return type == type_t::Null;
    }
    bool value_t::is_map() const {
        return type == type_t::Map;
    }
    bool value_t::is_array() const {
        return type == type_t::Arr;
    }
    bool value_t::is_number() const {
        return type == type_t::Num;
    }
    bool value_t::is_string() const {
        return type == type_t::Str;
    }
    bool value_t::is_bool() const {
        return type == type_t::Bool;
    }

    bool value_t::array(array_t &out) const {
        if (is_array()) {
            out = *val.arr;
            return true;
        }
        return false;
    }
    bool value_t::map(map_t &out) const {
        if (is_map()) {
            out = *val.map;
            return true;
        }
        return false;
    }
    bool value_t::number(number_t &out) const {
        if (is_number()) {
            out = val.num;
            return true;
        }
        return false;
    }
    bool value_t::string(string_t &out) const {
        if (is_string()) {
            out = *val.str;
            return true;
        }
        return false;
    }
    bool value_t::boolean(bool_t &out) const {
        if (is_bool()) {
            out = val.bl;
            return true;
        }
        return false;
    }

    array_t &value_t::array() const {
        if (is_array()) return *val.arr;
        else throw (std::string)"The value isn't an array.";
    }
    map_t &value_t::map() const {
        if (is_map()) return *val.map;
        else throw (std::string)"The value isn't a map.";
    }
    number_t value_t::number() const {
        if (is_number()) return val.num;
        else throw (std::string)"The value isn't a number.";
    }
    string_t &value_t::string() const {
        if (is_string()) return *val.str;
        else throw (std::string)"The value isn't a string.";
    }
    bool_t value_t::boolean() const {
        if (is_bool()) return val.bl;
        else throw (std::string)"The value isn't a bool.";
    }

    value_t::value_t() {
        this->type = type_t::Null;
    }
    value_t::value_t(const array_t &val) {
        this->type = type_t::Arr;
        this->val.arr = new array_t(val);
    }
    value_t::value_t(const map_t &val) {
        this->type = type_t::Map;
        this->val.map = new map_t(val);
    }
    value_t::value_t(const string_t &val) {
        this->type = type_t::Str;
        this->val.str = new string_t(val);
    }
    value_t::value_t(const char *val) {
        this->type = type_t::Str;
        this->val.str = new string_t(val);
    }
    value_t::value_t(bool_t val) {
        this->type = type_t::Bool;
        this->val.bl = val;
    }
    value_t::value_t(number_t val) {
        this->type = type_t::Num;
        this->val.num = val;
    }
    value_t::value_t(const value_t &other) {
        type = other.type;
        switch (other.type) {
            case type_t::Map:
                val.map = new map_t(*other.val.map);
                break;
            case type_t::Arr:
                val.arr = new array_t(*other.val.arr);
                break;
            case type_t::Str:
                val.str = new string_t(*other.val.str);
                break;
            default:
                val = other.val;
                break;
        }
    }
    value_t value_t::mk_arr() {
        return value_t(array_t());
    }
    value_t value_t::mk_map() {
        return value_t(map_t());
    }

    value_t::~value_t() {
        switch (type) {
            case type_t::Map:
                delete val.map;
                break;
            case type_t::Arr:
                delete val.arr;
                break;
            case type_t::Str:
                delete val.str;
                break;
            default:
                break;
        }
    }

    value_t::value_t(std::initializer_list<std::pair<std::string, value_t>> map):
        value_t(map_t(map)) { }

    value_t &value_t::operator=(const value_t &other) {
        type = other.type;
        switch (other.type) {
            case type_t::Map:
                val.map = new map_t(*other.val.map);
                break;
            case type_t::Arr:
                val.arr = new array_t(*other.val.arr);
                break;
            case type_t::Str:
                val.str = new string_t(*other.val.str);
                break;
            default:
                val = other.val;
                break;
        }
        return *this;
    }
    value_t &value_t::operator=(const char *other) {
        type = type_t::Str;
        val.str = new string_t(other);
        return *this;
    }

}
