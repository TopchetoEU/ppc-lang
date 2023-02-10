#pragma once

#include "treeifier/constr.hh"
#include "treeifier/parsers/helper.hh"

#ifdef PROFILE_debug
#include <iostream>
#endif

namespace ppc::tree::parse {

    struct named_t {
        virtual std::string name() = 0;
        virtual ~named_t() = default;
        
        #ifdef PROFILE_debug
        virtual void print() { std::cout << "(" << name() << ")"; }
        #endif
    };

    template <class ParserT, class ResT>
    class inspoint_t {
    private:
        std::map<std::string, std::unique_ptr<ParserT>> parsers;
    public:
        inspoint_t<ParserT, ResT> &add(const ParserT &parser, bool replace = false) {
            const std::string &name = parser.name();
            auto it = parsers.find(name);
            if (it != parsers.end()) {
                if (!replace) throw "The parser '" + name + "' is already in the group.";
                it->second = std::make_unique(parser);
            }
            else {
                parsers.emplace(name, std::make_unique(parser));
            }

            return *this;
        }

        bool operator()(ppc::tree::parse_ctx_t &ctx, size_t &i, std::unique_ptr<ResT> &out) const override {
            helper_t h(ctx, i);

            if (h.ended()) return false;

            for (std::pair<std::string, std::unique_ptr<ParserT>> &pair : parsers) {
                if (pair.second(ctx, i, out)) return true;
            }

            return false;
        }
    };
}