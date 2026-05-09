//rosterMap.cpp
// Daniel Koirala
// Modified version of roster example using map<string, list<string>>
//files from Nesterenko's site

#include <fstream>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <cstdlib>

using std::ifstream;
using std::string;
using std::list;
using std::cout; using std::endl;
using std::map;

// read one course roster
void readRoster(map<string, list<string>>& students, string fileName) {
    ifstream course(fileName);
    if (!course) {
        std::cerr << "Error opening file: " << fileName << endl;
        exit(1);
    }

    string first, last;
    string courseName = fileName.substr(0, fileName.find('.')); // e.g., "cs1"

    while(course >> first >> last) {
        string fullName = last + " " + first;
        students[fullName].push_back(courseName);
    }
    
}

void printRoster(const map<string, list<string>>& students) {
    cout << "ALL Students\n";
    cout << "last name, fist name: courses\n";

    for (const auto& [name, courses] : students) {
        cout << name << ": ";
        for (const auto& course : courses) {
            cout << course << " ";
        }
        cout << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        cout << "usage: " << argv[0] << " list of courses, dropouts last\n";
        exit(1);
    }

    map<string, list<string>> allStudents;

    // read courses
    for (int i = 1; i < argc; ++i) {
        readRoster(allStudents, argv[i]);
    }

    // remove dropouts
    ifstream drop(argv[argc - 1]);
    if (!drop) {
        std::cerr << "Error opening file: " << argv[argc - 1] << endl;
        exit(1);
    }

    string first, last;
    while (drop >> first >> last) {
        string fullName = last + " " + first;
        allStudents.erase(fullName);
    }
    drop.close();

    printRoster(allStudents);
}

