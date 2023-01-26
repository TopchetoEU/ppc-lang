#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std::string_literals;

struct project_t {
    std::string output;
    std::vector<std::string> deps;
};

std::string read_str(std::istream &f, const std::string &skip_chars, const std::string &end_chars, int &end_char) {
    std::vector<char> res {};
    int c;

    while (true) {
        c = f.get();
        auto a = end_chars.find(c);
        if (c == -1 || a != std::string::npos) {
            end_char = c;
            return "";
        }
        if ((a = skip_chars.find(c)) == std::string::npos) {
            f.unget();
            break;
        }
    }

    while (true) {
        c = f.get();
        if (c == -1 || end_chars.find(c) != std::string::npos) {
            end_char = c;
            break;
        }

        res.push_back(c);
    }
    while (true) {
        if (skip_chars.find(res.back()) != std::string::npos) res.pop_back();
        else break;
    }

    return std::string { res.begin(), res.end() };
}
std::string read_str(std::istream &f, const std::string &skip_chars, const std::string &end_chars) {
    int end_char;
    return read_str(f, skip_chars, end_chars, end_char);
}

project_t read_project(std::istream &f) {
    int end_ch;
    std::string name = read_str(f, " \v\t\r\n", "\n", end_ch);
    std::vector<std::string> deps {};

    if (name.length() == 0) {
        throw "The name of a project may not be empty."s;
    }

    if (end_ch == -1) {
        return project_t {
            .output = name,
            .deps = deps,
        };
    }

    if (name.find(',') != std::string::npos || name.find(' ') != std::string::npos) {
        throw "The name of a project may not contain spaces or commas."s;
    }

    while (true) {
        std::string dep = read_str(f, " \v\t\r\n", ",\n", end_ch);

        if (dep.find(' ') != std::string::npos) {
            throw "The name of a dependency may not contain spaces."s;
        }

        if (dep.length()) {
            deps.push_back(dep);
            if (end_ch == '\n') break;
        }
        if (end_ch == -1) break;
    }

    return project_t {
        .output = name,
        .deps = deps,
    };
}

void print_err(const std::string &error, const std::string &context) {
    std::cerr << context << ": " << error;
}

int main(int argc, const char *argv[]) {
    std::string proj_name = "";
    try {
        argc--;
        argv++;

        if (argc != 3) {
            throw "Incorrect usage. Syntax: [src-dir] [project-name] [output|deps]."s;
        }

        std::string proj_path = (std::string) argv[0] + "/" + argv[1] + ".proj";
        proj_name = argv[1];

        std::ifstream f { proj_path, std::ios_base::in };

        if (!f.is_open()) {
            throw "The project '"s + argv[1] + "' doesn't exist."s;
        }

        project_t project = read_project(f);
        f.close();

        if ((std::string) argv[2] == "output") {
            std::cout << project.output;
        }
        else if ((std::string) argv[2] == "deps") {
            for (std::string dep : project.deps) {
                std::cout << dep << " ";
            }
        }
        else {
            throw "Invalid command given. Available commands: output, deps."s;
        }

        std::cout << std::endl;

        return 0;
    }
    catch (std::string err) {
        if (proj_name.length()) std::cerr << "Error: " << proj_name << ": " << err << std::endl;
        else std::cerr << "Error: " << err << std::endl;
    }
    return -1;
}
