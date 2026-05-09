#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include "WordList.hpp"
#include "wordListDefinitions.cpp"

//helper to make lowercase copy of string for case-insensitive comparison
static std::string toLowerCopy(const std::string & s) {
    std::string result;
    result.reserve(s.size());
    for (unsigned char c : s) result.push_back(static_cast<char>(std::tolower(c)));
    return result;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream fin(argv[1], std::ios::in | std::ios::binary);
    if (!fin) {
        std::cerr << "Error: Could not open file " << argv[1] << "\n";
        return 1;
    }

    WordList w1; //count words into this list
    std::string token; //current word being built

        //lambda to flush current token to word list and clear it
    auto flush = [&](std::string& w) {
        if (w.empty()) return;
        //case-insensitive count
        std::string lower = toLowerCopy(w);
        if (lower != "skip") w1.addWord(lower);
        w.clear();
    };
    char c;
    while (fin.get(c)) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isalnum(uc)) token.push_back(static_cast<char>(uc));
        else flush(token);
    }
    flush(token); //flush any remaining token at EOF
    w1.print();
    fin.close();
    return 0;
}