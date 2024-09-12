#pragma once

#include <vector>
#include <memory>
#include "lang/common.hh"
#include "utils/message.hh"

#ifdef PROFILE_debug
#include <iostream>
#endif

namespace ppc::tree::constr {
    using namespace ppc::lang;
    using namespace ppc::messages;

    struct global_t;

    struct definition_t {
        virtual std::string name() const = 0;

        virtual bool verify(msg_stack_t &msg_stack, global_t &glob) const { return true; }
        virtual bool simplify(global_t &glob) const { return false; }
    };

    struct global_t {
        loc_nmsp_t nmsp;
        std::vector<std::unique_ptr<definition_t>> defs;
        std::vector<loc_nmsp_t> imports;

        #ifdef PROFILE_debug
        void print() const {
            std::cout << "Namespace: " << nmsp.to_string() << "\n";
            std::cout << "Imports:\n";
            for (auto &imp : imports) {
                std::cout << " - " << imp.to_string() << "\n";
            }
        }
        #endif
    };
}