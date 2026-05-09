#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <algorithm>

using std::ifstream;
using std::string; 
using std::list; 
using std::vector;
using std::cout; 
using std::endl;

// helper: find student entry in studentEntries
auto findStudent(vector<list<string>>& students, const string& name){
    return std::find_if(students.begin(), students.end(), 
        [&](const list<string>& entry){ return entry.front() == name; });
}

// read a course roster
void readRoster(vector<list<string>>& students, string fileName){
    ifstream course(fileName);
    string first, last;
    string courseName = fileName.substr(0, fileName.find('.')); // e.g. cs1
    
    while(course >> first >> last){
        string fullName = first + " " + last;
        auto it = findStudent(students, fullName);
        if(it == students.end()){           // student not found → create new
            list<string> entry;
            entry.push_back(fullName);
            entry.push_back(courseName);
            students.push_back(entry);
        } else {                            // student exists → add course
            it->push_back(courseName);
        }
    }
    course.close();
}

void removeDropouts(vector<list<string>>& students, string fileName){
    ifstream drop(fileName);
    string first, last;
    while(drop >> first >> last){
        string fullName = first + " " + last;
        auto it = findStudent(students, fullName);
        if(it != students.end())
            students.erase(it);
    }
    drop.close();
}

void printRoster(const vector<list<string>>& students){
    for(const auto& entry : students){
        auto it = entry.begin();
        cout << *it << ":"; 
        ++it;
        for(; it != entry.end(); ++it) cout << *it << " ";
        cout << endl;
    }
}

int main(int argc, char* argv[]){
    if(argc <= 2){ 
        cout << "usage: " << argv[0] << " list of courses, dropouts last\n";
        exit(1);
    }

    vector<list<string>> studentEntries;

    // read each course file except the last
    for(int i=1; i < argc-1; ++i){
        readRoster(studentEntries, argv[i]);
    }

    // remove dropouts
    removeDropouts(studentEntries, argv[argc-1]);

    // sort students
    std::sort(studentEntries.begin(), studentEntries.end(),
        [](const list<string>& a, const list<string>& b){
            return a.front() < b.front();
        });

    cout << "all students, dropouts removed and sorted\n";
    cout << "first name last name: courses enrolled\n";
    printRoster(studentEntries);

}
