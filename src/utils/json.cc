#include "utils/json.hh"
#include <sstream>

namespace ppc::data::json {
    std::string stringify(const data::value_t &val) {
        std::stringstream out;
        bool first = true;

        if (val.is_array()) {
            out << '[';

            for (const auto &el : val.array()) {
                if (!first) out << ',';
                first = false;
                out << stringify(el);
            }

            out << ']';
        }
        else if (val.is_map()) {
            out << '{';

            for (const auto &el : val.map()) {
                if (!first) out << ',';
                first = false;
                out << '"' << el.first << '"' << ':' << stringify(el.second);
            }

            out << '}';
        }
        else if (val.is_bool()) {
            if (val.boolean()) out << "true";
            else out << "false";
        }
        else if (val.is_null()) {
            out << "null";
        }
        else if (val.is_number()) {
            out << val.number();
        }
        else if (val.is_string()) {
            out << '"' << val.string() << '"';
        }

        return out.str();
    }
}