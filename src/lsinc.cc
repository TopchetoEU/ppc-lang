#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <cstring>

using namespace std::string_literals;
namespace fs = std::filesystem;

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}
static bool read_line(std::istream &str, std::string &res) {
    if (str.eof()) return false;
    std::getline(str, res);
    trim(res);
    return true;
}

fs::path find_file(fs::path original, fs::path include) {
    if (fs::exists(original)) return original;
    if (fs::exists(include / original)) return include / original;
    return original;
}

bool get_import(std::string line, fs::path root, fs::path &import) {
    if (line.rfind("#include") != 0) return false;
    line = line.substr(8);
    trim(line);
    if (line.length() < 3) throw "Invalid import syntax."s;
    if (line[0] == '<') return false;
    if (line[0] == '"') {
        if (line[line.length() - 1] != '"') throw "Invalid import syntax."s;
        auto path = line.substr(1, line.length() - 2);

        if (path.rfind("./") == 0 || path.rfind(".\\") == 0) {
            import = root / path.substr(2);
        }
        else import = path;

        return true;
    }

    throw "Invalid import syntax."s;
}

void get_imports(fs::path file, fs::path include, std::set<fs::path> &res) {
    static std::set<fs::path> parents = { };
    static std::vector<fs::path> tmp = { };

    if (!parents.emplace(file).second) {
        std::stringstream ss;
        ss << "Circular dependency encountered (";
        auto it = tmp.rbegin();

        for (; it != tmp.rend(); it++) {
            if (*it == file) break;
            ss << *it << "<-";
        }

        ss << *it << ")";

        parents.clear();
        tmp.clear();

        throw ss.str();
    }
    tmp.push_back(file);

    std::ifstream f(file.string());
    if (f.is_open() && !f.eof()) {
        std::string line;

        while (read_line(f, line)) {
            fs::path import;
            if (get_import(line, file.parent_path(), import)) {
                auto child = find_file(import, include);
                res.emplace(child);
                get_imports(child, include, res);
            }
        }
    }

    parents.erase(file);
    tmp.pop_back();
}

int main(int argc, char **argv) {
    try {
        argc--;
        argv++;

        if (argc == 0) {
            throw "Incorrect usage. Syntax: [inc-path] [files...]."s;
            return -1;
        }

        fs::path inc(argv[0]);
        std::set<fs::path> res;

        for (int i = 1; i < argc; i++) {
            fs::path file(argv[i]);
            get_imports(file, inc, res);
        }

        for (auto &i : res) {
            std::cout << i.string() << "\n";
        }


        return 0;
    }
    catch (std::string err) {
        std::cerr << "error: lsinc: " << err << std::endl;
    }

    return -1;
}