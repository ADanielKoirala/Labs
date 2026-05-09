// making sure that duplicate and sick animals are removed from all enclosures in a zoo 
// demonstrates the usage of STL algorithms adapted from Pro C++ 
// Wayne Cheng
// 10/2/2015 
#include <algorithm>
#include <iostream> 
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_set>

  
using namespace std; 

// this is similar to typedef 
using EnclosurePair = pair<string, list<string>>; 
using ZooMap = map<string, list<string>>;

// removes duplicate and sick animals form ZooMap 
void auditEnclosures(ZooMap& animalsByEnclosure, const list<string>& sickAnimals); 

// returns a set of duplicate animals 
multiset<string> getDuplicates(const ZooMap& animalsByEnclosure); 

int main() 
{ 
	// initialize map ZooMap 
	ZooMap animals = {
                { "Orange", { "aardvark", "lion", "buffalo", "wildcat", "lion", "eagle" } }, 
				{ "Green", { "elephant", "zebra", "aardvark", "bear", "alligator"} }, 
				{ "Yellow", { "goose", "alligator", "flamingo", "goose", } },
                { "Blue", { "shark", "dolphin", "alligator", "turtle", "eagle", "penguin"} }, 
                { "Red", { "deer", "duck", "wildcat", "lion" } }
             }; 
	
	list<string> sickAnimals = { "buffalo", "wildcat", "eagle"}; 
	
	// local lambda expression to print a enclosure 
	auto printEnclosure = [](const EnclosurePair& enclosure) 
	{ 
		cout << enclosure.first << ":"; 
		for (const auto&s : enclosure.second) cout << " " << s;
        cout << '\n'; 
	}; 


	cout << "Before audit:\n";

	for_each(animals.cbegin(), animals.cend(), printEnclosure); 
	cout << '\n';

	auditEnclosures(animals, sickAnimals); 

	cout << "After audit:\n"; 
    for_each(animals.cbegin(), animals.cend(), printEnclosure); 
	cout << '\n';
    return 0;
} 

// expects a map of string/list pairs keyed on enclosure names 
// and containing lists of all the animals in those enclosures 
// 
// removes from each list any name on the sickAnimals list and 
// any name that is found in any other enclosure 
void auditEnclosures(ZooMap& animalsByEnclosure, const list<string>& sickAnimals) { 
	// get all the duplicate names 
	multiset<string> extras = getDuplicates(animalsByEnclosure); 
	
	//sick animals, remove
    unordered_set<string> sickSet(sickAnimals.begin(), sickAnimals.end());
	
	for (auto& [enclosureName, animals] : animalsByEnclosure) {
        auto it = remove_if(animals.begin(), animals.end(),
            [&extras, &sickSet](const string& name) {
                //always remove if sick
                if (sickSet.count(name)) return true;

                //remove if duplicate in extras
                auto dupIt = extras.find(name);
                if (dupIt != extras.end()) {
                    extras.erase(dupIt); //remove one occurrence
                    return true;  
            }
            return false;
        });
        animals.erase(it, animals.end());
    }
}

    

// returns a set of all animal names that appear in more than one list in the map 
// the implementation generates one large list of all the animal names from all the 
// lists in the map, sorts it, then finds all duplicates in the sorted list with adjacent_find() 
multiset<string> getDuplicates(const ZooMap& animalsByEnclosure) { 
    // Collect all the names from all the lists into one big list
    map<string, size_t> counts;
    for (const auto& [encName, animals] : animalsByEnclosure) {
        for (const auto& a : animals) ++counts[a];
    }
    //build multiset of extras: count -1 copies for any count >=2
    multiset<string> extras;
    for (const auto& [animal, count] : counts) {
        if (count >= 2) {
            for (size_t i =0; i < count -1; ++i) extras.insert(animal);
        }
    }
    return extras;
}
	