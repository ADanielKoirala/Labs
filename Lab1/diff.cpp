/*
Daniel Koirala
Lab 1
CSIII - 12103
08/25/2025
*/
#include <iostream>
#include <fstream>
#include <string>

void compareFiles (const char *file1Name, const char *file2Name) {
    std::ifstream file1(file1Name);
    std::ifstream file2(file2Name);

    if (!file1.is_open() || !file2.is_open()) {
        std::cerr << "Error: Unable to open files" << std::endl;
        return;
    }

    std::string line1, line2; //read line from each file
    int lineNumber =0;

    //loop until EOF for both files
    while (true) {
        bool file1HasLine = (std::getline(file1, line1) ? true : false);
        bool file2HasLine = (std::getline(file2, line2) ? true : false);
        if (!file1HasLine && !file2HasLine) break; //reach EOF

        lineNumber++;

        if (!file1HasLine) line1.clear();

        if (!file2HasLine) line2.clear();

        if (line1 != line2) { //check if two lines differ
            std::cout << file1Name << ": " << lineNumber << ": " << line1 << '\n';
            std::cout << file2Name << ": " << lineNumber << ": " << line2 << '\n';

            size_t diffIndex = 0;
            //index of first differing character
            while (diffIndex < line1.size() && diffIndex < line2.size() && line1[diffIndex] == line2[diffIndex]) {
                diffIndex++;
            }
            //if one line is shorter than other, put the caret at the end of shorter line
            if (diffIndex == line1.size() || diffIndex == line2.size()) {
                if (line1.size() < line2.size()) {
                    diffIndex = line1.size();
                } else {
                    diffIndex = line2.size();
                }
            }
            //prefix so output format and differing index length align
            std::string prefix = std::string(file2Name) + ": " + std::to_string(lineNumber) + ": ";
            size_t offset = prefix.size();

            std::string caretLine(offset + diffIndex, ' ');
            caretLine += '^';
            std::cout << caretLine << '\n';
        }
    }

    file1.close();
    file2.close();

}
//needs two files to compare
int main(int argc, char *argv[]) {
    if (argc !=3) {
        std::cerr << "needs two files to compare" << std::endl;
        return 1;
    }

    compareFiles (argv[1], argv[2]);

    return 0;
}