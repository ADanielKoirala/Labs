// demoes hashmap implementation
// Wayne Cheng
// 4/10/2018

// modified by Daniel Koirala for CSIII

#include "hashmap.h"
#include <iostream>
#include <string>
#include <cassert>

using std::cin;
using std::cout;
using std::endl;
using std::string;

using IntMap = hashmap<int, int>;
using EmpMap = hashmap<int, string>;

int main() {
    {
        cout << "== Basic safe insert & find ==\n";
        IntMap m;
        auto r1 = m.insert({4, 40});
        cout << "insert {4,40}: inserted=" << std::boolalpha << r1.second
             << ", value=" << r1.first->second << "\n";

        auto r2 = m.insert({6, 60});
        cout << "insert {6,60}: inserted=" << r2.second
             << ", value=" << r2.first->second << "\n";

        auto r3 = m.insert({4, 444}); // duplicate key, should not insert
        cout << "insert {4,444}: inserted=" << r3.second
             << ", value (existing)=" << r3.first->second << "\n";

        assert(r3.second == false);
        assert(r3.first->second == 40);

        auto x = m.find(4);
        cout << (x ? "find(4) -> " + std::to_string(x->second)
                   : "find(4) -> nullptr")
             << "\n";
    }

    {
        cout << "\n== operator[] without double find ==\n";
        IntMap m;
        m[4] = 35; // creates
        m[4] = 60; // updates

        auto x = m.find(4);
        cout << "4 maps to " << (x ? std::to_string(x->second)
                                   : string("N/A"))
             << "\n";
        assert(x && x->second == 60);
    }

    {
        cout << "\n== erase(pair<ptr,bool>) semantics ==\n";
        IntMap m;
        for (int k : {1, 2, 3, 104, 205}) { // likely to spread across buckets
            m.insert({k, k * 10});
        }
        cout << "size before erase=" << m.size() << "\n";

        // erase something with a successor in the same bucket or later bucket
        auto e1 = m.erase(2);
        cout << "erase(2): erased=" << std::boolalpha << e1.second
             << ", nextPtr="
             << (e1.first ? std::to_string(e1.first->first)
                          : string("nullptr"))
             << "\n";
        assert(e1.second == true);

        // erase something else, possibly the last element overall
        auto e2 = m.erase(205);
        cout << "erase(205): erased=" << std::boolalpha << e2.second
             << ", nextPtr="
             << (e2.first ? std::to_string(e2.first->first)
                          : string("nullptr"))
             << "\n";
        assert(e2.second == true);

        // erase non-existent
        auto e3 = m.erase(999);
        cout << "erase(999): erased=" << std::boolalpha << e3.second
             << ", nextPtr (unspecified)="
             << (e3.first ? "non-null" : "nullptr")
             << "\n";
        assert(e3.second == false);

        cout << "size after erases=" << m.size() << "\n";
    }

    {
        cout << "\n== rehash(n) grows bucket count and keeps elements ==\n";
        EmpMap employees;
        employees[123] = "Mike";
        employees[345] = "Charlie";
        employees[192] = "Joe";
        employees[752] = "Paul";
        employees[328] = "Peter";

        cout << "bucket_count before=" << employees.bucket_count() << "\n";
        size_t grow_to = employees.bucket_count() * 2 + 1;
        employees.rehash(grow_to);
        cout << "bucket_count after=" << employees.bucket_count() << "\n";

        // verify contents survived
        for (int id : {123, 345, 192, 752, 328}) {
            auto p = employees.find(id);
            assert(p && "element lost after rehash");
        }

        // small interactive exercise using safe insert feel
        int num;
        string name;
        cout << "\nEnter new employee number: ";
        if (!(cin >> num)) return 0;
        cout << "Enter new employee name: ";
        if (!(cin >> name)) return 0;

        auto ins = employees.insert({num, name});
        if (ins.second) {
            cout << "Inserted new employee: "
                 << ins.first->first << " -> " << ins.first->second << "\n";
        } else {
            cout << "Employee existed, kept name: "
                 << ins.first->first << " -> " << ins.first->second << "\n";
        }

        cout << "Enter employee number to look for: ";
        int look;
        if (!(cin >> look)) return 0;
        auto it = employees.find(look);
        if (it) cout << it->first << ":" << it->second << "\n";
        else    cout << "employee not found\n";

        cout << "Enter employee number to fire: ";
        int fire;
        if (!(cin >> fire)) return 0;
        auto er = employees.erase(fire);
        if (er.second) {
            cout << "Fired. Next element is "
                 << (er.first
                     ? std::to_string(er.first->first) + ":" + er.first->second
                     : string("nullptr"))
                 << "\n";
        } else {
            cout << "No such employee.\n";
        }

        auto removed = employees.find(fire);
        if (removed == nullptr) cout << "Confirmed removed (or absent).\n";
    }

    cout << "\nAll tests completed.\n";
    return 0;
}
