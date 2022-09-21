#include "data.hh"

bool ppc::data::value_t::is_null() const {
    return type == type_t::Null;
}
bool ppc::data::value_t::is_map() const {
    return type == type_t::Map;
}
bool ppc::data::value_t::is_array() const {
    return type == type_t::Arr;
}
bool ppc::data::value_t::is_number() const {
    return type == type_t::Num;
}
bool ppc::data::value_t::is_string() const {
    return type == type_t::Str;
}
bool ppc::data::value_t::is_bool() const {
    return type == type_t::Bool;
}

bool ppc::data::value_t::array(ppc::data::array_t &out) const {
    if (is_array()) {
        out = *val.arr;
        return true;
    }
    return false;
}
bool ppc::data::value_t::map(ppc::data::map_t &out) const {
    if (is_map()) {
        out = *val.map;
        return true;
    }
    return false;
}
bool ppc::data::value_t::number(ppc::data::number_t &out) const {
    if (is_number()) {
        out = val.num;
        return true;
    }
    return false;
}
bool ppc::data::value_t::string(ppc::data::string_t &out) const {
    if (is_string()) {
        out = *val.str;
        return true;
    }
    return false;
}
bool ppc::data::value_t::boolean(ppc::data::bool_t &out) const {
    if (is_bool()) {
        out = val.bl;
        return true;
    }
    return false;
}

const ppc::data::array_t &ppc::data::value_t::array() const {
    if (is_array()) return *val.arr;
    else throw (std::string)"The value isn't an array.";
}
const ppc::data::map_t &ppc::data::value_t::map() const {
    if (is_map()) return *val.map;
    else throw (std::string)"The value isn't a map.";
}
ppc::data::number_t ppc::data::value_t::number() const {
    if (is_number()) return val.num;
    else throw (std::string)"The value isn't a number.";
}
const ppc::data::string_t &ppc::data::value_t::string() const {
    if (is_string()) return *val.str;
    else throw (std::string)"The value isn't a string.";
}
ppc::data::bool_t ppc::data::value_t::boolean() const {
    if (is_bool()) return val.bl;
    else throw (std::string)"The value isn't a bool.";
}

ppc::data::value_t::value_t() {
    this->type = type_t::Null;
}
ppc::data::value_t::value_t(const ppc::data::array_t &val) {
    this->type = type_t::Arr;
    this->val.arr = new array_t { val };
}
ppc::data::value_t::value_t(const ppc::data::map_t &val) {
    this->type = type_t::Map;
    this->val.map = new map_t { val };
}
ppc::data::value_t::value_t(const ppc::data::string_t &val) {
    this->type = type_t::Str;
    this->val.str = new string_t { val };
}
ppc::data::value_t::value_t(ppc::data::bool_t val) {
    this->type = type_t::Bool;
    this->val.bl = val;
}
ppc::data::value_t::value_t(ppc::data::number_t val) {
    this->type = type_t::Num;
    this->val.num = val;
}
ppc::data::value_t::value_t(const ppc::data::value_t &other) {
    type = other.type;
    switch (other.type) {
        case type_t::Map:
            val.map = new map_t { *other.val.map };
            break;
        case type_t::Arr:
            val.arr = new array_t { *other.val.arr };
            break;
        case type_t::Str:
            val.str = new string_t { *other.val.str };
            break;
        default:
            val = other.val;
            break;

    }
}
ppc::data::value_t::~value_t() {
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

