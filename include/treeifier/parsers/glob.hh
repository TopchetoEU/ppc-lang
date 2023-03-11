#pragma once

#include "lang/common.hh"
#include "treeifier/tokenizer.hh"
#include "treeifier/constr.hh"
#include "treeifier/constructs/glob.hh"
#include "treeifier/parsers/inspoint.hh"

namespace ppc::tree::parse {
    using namespace constr;

    class glob_parser_t {
    public:
        bool operator()(parse_ctx_t &ctx, global_t &out) const;
    };
}