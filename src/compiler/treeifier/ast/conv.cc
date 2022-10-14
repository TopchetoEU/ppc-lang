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
        return { conv::map_to_loc(map["location"].map()), map["content"].string() };
    }

    data::map_t loc_to_map(const location_t &loc) {
        data::map_t res = {
            { "$_name", "$_loc" },
        };

        res["filename"] = loc.filename;
        if (loc.start != -1u) res["start"] = (float)loc.start;
        if (loc.start != -1u) res["line"] = (float)loc.line;
        if (loc.code_start != -1u) res["code_start"] = (float)loc.code_start;
        if (loc.length != -1u) res["length"] = (float)loc.length;

        return res;
    }
    location_t map_to_loc(const data::map_t &map) {
        location_t res(map["filename"].string());

        if (map.has("start")) {
            if (map["start"].is_number()) res.start = (size_t)map["start"].number();
            else throw "Expected key 'start' to be a number.";
        }
        if (map.has("length")) {
            if (map["length"].is_number()) res.length = (size_t)map["length"].number();
            else throw "Expected key 'length' to be a number.";
        }
        if (map.has("code_start")) {
            if (map["code_start"].is_number()) res.code_start = (size_t)map["code_start"].number();
            else throw "Expected key 'code_start' to be a number.";
        }
        if (map.has("line")) {
            if (map["line"].is_number()) res.line = (size_t)map["line"].number();
            else throw "Expected key 'line' to be a number.";
        }

        return res;
    }

    data::map_t nmsp_to_map(const loc_namespace_name_t &nmsp) {
        data::map_t res;

        auto arr = res["content"].array({});

        for (const auto &segment : nmsp) {
            arr.push({
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
