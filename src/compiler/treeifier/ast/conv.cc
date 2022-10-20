#include <sstream>
#include "compiler/treeifier/ast.hh"

namespace ppc::comp::tree::ast::conv {
    data::map_t identifier_to_map(const located_t<std::string> &loc) {
        return {
            { "location", conv::loc_to_map(loc.location) },
            { "content", loc },
            { "$_name", "$_identifier" },
        };
    }
    located_t<std::string> map_to_identifier(const data::map_t &map) {
        return { conv::map_to_loc(map["location"].string()), map["content"].string() };
    }

    data::string_t loc_to_map(const location_t &loc) {
        std::stringstream res;
        res << loc.filename << ':' << loc.line + 1 << ':' << loc.start + 1 << ':' << loc.code_start + 1 << ':' << loc.length + 1;
        return res.str();
    }
    location_t map_to_loc(const data::string_t &map) {
        std::stringstream res;
        res.str(map);

        std::string filename;
        std::string line;
        std::string start;
        std::string code_start;
        std::string length;

        std::getline(res, filename, ':');
        std::getline(res, line, ':');
        std::getline(res, start, ':');
        std::getline(res, code_start, ':');
        std::getline(res, length, ':');

        return { filename, std::stoull(line) - 1, std::stoull(start) - 1, std::stoull(code_start) - 1, std::stoull(length) - 1 };
    }

    data::map_t nmsp_to_map(const loc_namespace_name_t &nmsp) {
        data::map_t res;

        auto arr = res["content"].array({});

        for (const auto &segment : nmsp) {
            arr.push_back({
                { "location", loc_to_map(segment.location) },
                { "content", segment },
                { "$_name", "$_nmsp" },
            });
        }

        return res;
    }
    loc_namespace_name_t map_to_nmsp(const data::map_t &map) {
        loc_namespace_name_t res;

        for (const auto &segment : map["content"].array()) {
            try {
                auto val = map_to_identifier(segment.map());
                res.push_back(val);
            }
            catch (const message_t &) {
                throw "'content' of a namespace map must contain only identifiers.";
            }
        }

        return res;
    }
}
