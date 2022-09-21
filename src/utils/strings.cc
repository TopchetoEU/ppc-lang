#include <sstream>
#include "utils/strings.hh"

using namespace ppc;

std::vector<std::string> str::split(const std::string &splittable, std::initializer_list<char> splitters, bool remove_empty_entries) {
    std::stringstream buff;
    std::vector<std::string> res;

    for (char c : splittable) {
        if (std::string { splitters }.find(c) == -1u) {
            buff << c;
        }
        else {  
            if (!buff.str().empty() || !remove_empty_entries) {
                res.push_back(buff.str());
                buff = { };
            }
        }
    }

    if (!buff.str().empty() || !remove_empty_entries) {
        res.push_back(buff.str());
    }

    return res;
}

std::string str::trim(std::string splittable, std::initializer_list<char> splitters) {
    auto split = std::string { splitters };

    while (!splittable.empty() && split.find(splittable[0]) != -1u) {
        splittable = splittable.substr(1);
    }
    while (!splittable.empty() && split.find(splittable[splittable.length() - 1]) != -1u) {
        splittable = splittable.substr(0, splittable.length() - 1);
    }

    return splittable;
}
