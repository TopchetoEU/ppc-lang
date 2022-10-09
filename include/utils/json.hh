#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "utils/data.hh"

namespace ppc::data::json {
    std::string stringify(const data::value_t &map);
}