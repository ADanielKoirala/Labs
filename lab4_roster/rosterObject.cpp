// rosterObject.cpp
// Modified version with Student storing courses
// Includes move constructor demonstration
// Mikhail Nesterenko style, adapted

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
using std::move;

class Student {
public:
    Student(string first, string last) 
        : firstName_(first), lastName_(last) {}

    // copy constructor
    Student(const Student& other) = default;

    // move constructor
    Student(Student&& other) noexcept
        : firstName_(move(other.firstName_)),
          lastName_(move(other.lastName_)),
          courses_(move(other.courses_)) {}

    //copy assignment
    Student& operator=(const Student& other) = default;

    //move assignment
    Student& operator=(Student&& other) noexcept {
        if (this != &other) {
            firstName_ = move(other.firstName_);
            lastName_ = move(other.lastName_);
            courses_ = move(other.courses_);
        }
        return *this;
    }

    // add a course
    void addCourse(const string& course) { courses_.push_back(course); }

    // return "First Last"
    string fullName() const { return firstName_ + " " + lastName_; }

    // return formatted string
    string print() const {
        string out = fullName() + ":";
        for (const auto& c : courses_) out += c + " ";
        return out;
    }

    // equality for remove() and unique()
    friend bool operator==(const Student& l, const Student& r) {
        return l.firstName_ == r.firstName_ && l.lastName_ == r.lastName_;
    }

    // comparison for sort()
    friend bool operator<(const Student& l, const Student& r) {
        return l.lastName_ < r.lastName_ ||
               (l.lastName_ == r.lastName_ && l.firstName_ < r.firstName_);
    }

private:
    string firstName_, lastName_;
    list<string> courses_;
};

// helper: find student in vector
auto findStudent(vector<Student>& students, const string& name) {
    return std::find_if(students.begin(), students.end(),
        [&](const Student& s) { return s.fullName() == name; });
}

// read one course roster
void readRoster(vector<Student>& students, string fileName) {
    ifstream course(fileName);
    string first, last;
    string courseName = fileName.substr(0, fileName.find('.')); // e.g., "cs1"

    while (course >> first >> last) {
        string fullName = first + " " + last;
        auto it = findStudent(students, fullName);
        if (it == students.end()) {
            Student s(first, last);
            s.addCourse(courseName);
            students.push_back(move(s)); // move constructor invoked
        } else {
            it->addCourse(courseName);
        }
    }
    course.close();
}

// remove dropouts
void removeDropouts(vector<Student>& students, string fileName) {
    ifstream drop(fileName);
    string first, last;
    while (drop >> first >> last) {
        string fullName = first + " " + last;
        auto it = findStudent(students, fullName);
        if (it != students.end()) students.erase(it);
    }
    drop.close();
}

// print final roster
void printRoster(const vector<Student>& students) {
    for (const auto& s : students) cout << s.print() << endl;
}

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        cout << "usage: " << argv[0] << " list of courses, dropouts last\n";
        exit(1);
    }

    vector<Student> allStudents;

    // read courses
    for (int i = 1; i < argc - 1; ++i) {
        readRoster(allStudents, argv[i]);
    }

    // remove dropouts
    removeDropouts(allStudents, argv[argc - 1]);

    // sort by name
    std::sort(allStudents.begin(), allStudents.end());

    // print results
    cout << "all students, dropouts removed and sorted\n";
    cout << "first name last name: courses enrolled\n";
    printRoster(allStudents);

    return 0;
}
