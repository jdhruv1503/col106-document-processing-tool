#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <regex>

namespace fs = std::filesystem;

int main() {
    fs::path dir_path = "./txtfiles"; // replace with your directory path
    std::ofstream out_file("corpus.txt");

    for (const auto &entry : fs::directory_iterator(dir_path)) {
        if (entry.path().extension() == ".txt") {
            std::ifstream in_file(entry.path());
            std::string line;
            while (std::getline(in_file, line)) {
                line = std::regex_replace(line, std::regex(R"(\('[0-9]+', [0-9]+, [0-9]+, [0-9]+, [0-9]+\))"), "");
                std::replace(line.begin(), line.end(), '\n', ' ');
                out_file << line << ' ';
            }
        }
        std::cout << "processed" << entry.path() << std::endl;
    }

    out_file.close();

    return 0;
}
