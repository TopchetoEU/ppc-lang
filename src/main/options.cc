#include <set>
#include "./opions.hh"

using namespace ppc;

bool check_shorthand(std::string &option, options::flag_t const &flag) {
    if (option.size() < 2 || option[0] != '-') return false;

    if (option.size() == 2 && std::string { flag.shorthands }.find(option[1]) != -1u) {
        option = "";
        return true;
    }
    else if (flag.match_type == options::MATCH_PREFIX) {
        if (option.length() < 3 || option[2] != '=') return false;
        option = option.substr(3);
        return true;
    }
    return false;
}
bool check_name(std::string &option, options::flag_t const &flag) {
    if (option.size() > 2 && option[0] == '-' && option[1] == '-') {
        std::string candidate = option.substr(2);
        if (flag.match_type == options::MATCH_WHOLE) {
            if (candidate == flag.name) {
                option = "";
                return true;
            }
        }
        else {
            if (candidate.find(flag.name) == 0) {
                candidate = candidate.substr(flag.name.length());
                if (candidate.length() > 0) {
                    if (candidate[0] == '=') {
                        option = candidate.substr(1);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void ppc::options::parser_t::add_flag(flag_t const &flag) {
    flags.push_back(flag);
}
void ppc::options::parser_t::clear_flags() {
    flags.clear();
}

bool ppc::options::parser_t::parse(std::string const &option, messages::msg_stack_t &msg_stack, data::map_t &conf) {
    if (option.empty()) return false;

    std::string opt = option;

    for (auto const &flag : flags) {
        if (check_name(opt, flag) || check_shorthand(opt, flag)) {
            flag.execute(*this, opt, msg_stack);
            return true;
        }
    }
    return false;
}
