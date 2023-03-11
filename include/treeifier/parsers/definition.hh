#pragma once

#include "lang/common.hh"
#include "treeifier/tokenizer.hh"
#include "treeifier/constr.hh"
#include "treeifier/constructs/glob.hh"

namespace ppc::tree::parse {
    class definition_parser_t {
    public:
        virtual bool operator()(parse_ctx_t &ctx, size_t &res_i, constr::definition_t &res) const = 0;
    };
}