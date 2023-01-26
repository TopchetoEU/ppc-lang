#include "treeifier/constr.hh"
#include "treeifier/constr/helper.hh"
#include "treeifier/constr/nmsp.hh"

namespace ppc::tree::constr {
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

        bool operator()(ast_ctx_t &ctx, size_t &i, std::unique_ptr<ResT> &out) const override {
            helper_t h(ctx, i);

            if (h.ended()) return false;

            for (std::pair<std::string, std::unique_ptr<ParserT>> &pair : parsers) {
                ResT res;

                if (pair.second(ctx, i, res)) {
                    out = std::make_unique<ResT>(res);
                    return true;
                }
            }

            return false;
        }
    };
}