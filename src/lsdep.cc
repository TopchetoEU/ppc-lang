#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <filesystem>
#include <algorithm>

using namespace std::string_literals;
using std::size_t;
namespace fs = std::filesystem;

fs::path proj_dir;
bool recursive = false;
std::string prefix = "", suffix = "";

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

void read_project_rec(const std::string &name, std::set<std::string> &res, bool rec = false) {
    static std::set<std::string> prev;

    if (!prev.emplace(name).second) throw name + ": Circular dependency detected.";

    std::ifstream f(proj_dir / name);
    std::string line;

    while (read_line(f, line)) {
        if (line.find('#') != std::string::npos) {
            line = line.substr(0, line.find('#'));
        }

        trim(line);

        if (line.size() == 0) continue;
        if (line == name) throw name + ": Depends on itself.";

        res.emplace(line);
        if (rec) read_project_rec(line, res);
    }

    prev.erase(name);
}

void print_err(const std::string &error, const std::string &context) {
    std::cerr << context << ": " << error;
}

void set_transform(const std::string &val) {
    size_t i = 0;
    bool last_percent = false;
    bool is_prefix = true;
    prefix = "", suffix = "";

    for (; i < val.length(); i++) {
        if (is_prefix) {
            if (val[i] == '*') {
                if (last_percent) {
                    last_percent = false;
                }
                else {
                    last_percent = true;
                    continue;
                }
            }
            else if (last_percent) {
                is_prefix = false;
                last_percent = false;
            }
        }

        if (is_prefix) prefix += val[i];
        else suffix += val[i];
    }
}

bool process_arg(std::string arg) {
    if (arg.find("--") != 0) return false;
    arg = arg.substr(2);

    if (arg == "rec") {
        recursive = true;
        return true;
    }
    else if (arg.find("transform=") == 0) {
        arg = arg.substr(10);
        set_transform(arg);
    }
    else if (arg.find("dir=") == 0) {
        arg = arg.substr(4);
        proj_dir = arg;
    }
    else {
        throw "Incorrect usage. Syntax: [--rec] [--transform=prefix%suffix] [--dir=dir] [projects...]"s;
    }
    return true;
}

int main(int argc, const char *argv[]) {
    try {
        argc--;
        argv++;

        std::set<std::string> deps;

        while (argc > 0 && process_arg(argv[0])) {
            argc--;
            argv++;
        }
        while (argc > 0) {
            // if (recursive) read_project_rec(argv[0], deps);
            // else read_project(argv[0], deps);
            read_project_rec(argv[0], deps, recursive);
            argc--;
            argv++;
        }

        for (auto &dep : deps) {
            std::cout << prefix + dep + suffix << std::endl;
        }

        return 0;
    }
    catch (std::string err) {
        std::cerr << "error: lsdep: " << err << std::endl;
    }
    return -1;
}
