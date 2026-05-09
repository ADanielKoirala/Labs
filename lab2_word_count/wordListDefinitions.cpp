#include "WordList.hpp"
#include <algorithm>
#include <iostream>

// WordOccurrence class definitions
WordOccurrence::WordOccurrence(const std::string& word, int num) : word_(word), num_(num) {}
bool WordOccurrence::matchWord(const std::string &w) {return word_ == w;}
void WordOccurrence::increment() {++num_;}
std::string WordOccurrence::getWord() const {return word_;}
int WordOccurrence::getNum() const {return num_;}

// WordList class definitions
WordList::WordList() : wordArray_(nullptr), size_(0) {}

//copy constructor, deep copy of other
WordList::WordList(const WordList& other) : wordArray_(nullptr), size_(0) {
    if (other.size_ > 0) {
        wordArray_ = new WordOccurrence[other.size_];
        for (int i = 0; i< other.size_; ++i) wordArray_[i] = other.wordArray_[i];
        size_ = other.size_;   
    }
}

WordList::~WordList() { delete[] wordArray_;}

//assignment operator using copy-and-swap idiom
WordList& WordList::operator=(WordList other) {
    std::swap(wordArray_, other.wordArray_);
    std::swap(size_, other.size_);
    return *this;
}

void WordList::addWord(const std::string& w) {
    //check if word already exists
    for (int i = 0; i < size_; ++i) {
        if (wordArray_[i].matchWord(w)) {
            wordArray_[i].increment();
            return;
        }
    }
    //if not, add the new word
    WordOccurrence* newArray = new WordOccurrence[size_ + 1];
    for (int i = 0; i < size_; ++i) newArray[i] = wordArray_[i];
    newArray[size_] = WordOccurrence(w, 1);
    delete[] wordArray_;
    wordArray_ = newArray;
    ++size_;    
}

bool equal(const WordList& wl1, const WordList& wl2) {
    if (wl1.size_ != wl2.size_) return false;
    if (wl1.size_ == 0) return true;
    //temp copies to sort without changing original
    WordOccurrence* ca = new WordOccurrence[wl1.size_];
    WordOccurrence* cb = new WordOccurrence[wl2.size_];
    for (int i = 0; i < wl1.size_; ++i) ca[i] = wl1.wordArray_[i];
    for (int i = 0; i < wl2.size_; ++i) cb[i] = wl2.wordArray_[i];
    //sort both litsts by word
    auto goByWord = [](const WordOccurrence& a, const WordOccurrence& b) {
        return a.getWord() < b.getWord();
    };
    std::sort(ca, ca + wl1.size_, goByWord);
    std::sort(cb, cb + wl2.size_, goByWord);
    //compare by word and count
    bool same = true;
    for (int i = 0; i < wl1.size_; ++i) {
        if (ca[i].getWord() != cb[i].getWord() || ca[i].getNum() != cb[i].getNum()) {
            same = false;
            break; 
        }
    }
    delete[] ca;
    delete[] cb;
    return same;
    
}
//print using increasing occurrences, then alphabetically
void WordList::print() const {
    if (size_ == 0) {
        std::cout << "No words to display." << std::endl;
        return;
    }
    //temp array to sort without changing original
    WordOccurrence* sortedArray = new WordOccurrence[size_];
    for (int i = 0; i < size_; ++i) sortedArray[i] = wordArray_[i];
    //smaller first, then alphabetically if equal
    std::sort(sortedArray, sortedArray + size_, [](const WordOccurrence& a, const WordOccurrence& b) {
        if (a.getNum() != b.getNum()) return a.getNum() < b.getNum();
        return a.getWord() < b.getWord();
    });

    for (int i = 0; i < size_; ++i) {
        std::cout << sortedArray[i].getWord() << ": " << sortedArray[i].getNum() << "\n";
    }
    delete[] sortedArray;
}
