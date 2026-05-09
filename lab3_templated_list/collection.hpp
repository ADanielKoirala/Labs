#ifndef COLLECTION_HPP_
#define COLLECTION_HPP_

#include <iostream>
#include <string>
#include "list.hpp"

template <typename T>
class Collection{
public:
    //default constructor
    Collection(): head_(nullptr), size_(0) {}

    //copy constructor
    Collection(const Collection& other);

    //destructor
    ~Collection();

    //assignmnet operator
    Collection& operator=(const Collection& other);

    //add element to back
    void add(const T& element);

    //remove all instances of element
    void remove(const T& element);

    //return last element
    T last() const;

    //print collection elements
    void print() const;

    //equality operator (not a template as per instructions)
    
    friend bool equal(const Collection<int>&, const Collection<int>&);
    friend bool equal(const Collection<std::string>&, const Collection<std::string>&);
    friend bool equal(const Collection<char>&, const Collection<char>&);   
    friend bool equal(const Collection<double>&, const Collection<double>&);
    friend bool equal(const Collection<Collection<char>>&, const Collection<Collection<char>>&);

private:
    node<T> *head_; //pointer to the head of the list
    int size_; //number of elements in the collection
};

//copy constructor
template <typename T>
Collection<T>::Collection(const Collection<T>& other): head_(nullptr), size_(0) {
    if(!other.head_) return; //other is empty
    //copy first
    head_ = new node<T>;
    head_->setData(other.head_->getData());

    node<T> *current = head_;
    node<T> *otherCurrent = other.head_->getNext();
    //copy rest
    while (otherCurrent) {
        node<T> *newNode = new node<T>;
        newNode->setData(otherCurrent->getData());
        current->setNext(newNode);
        current = newNode;
        otherCurrent = otherCurrent->getNext();
    }
    size_ = other.size_;
}

//destructor
template <typename T>
Collection<T>::~Collection() {
    node<T> *current = head_;
    while (current) {
        node<T> *temp = current;
        current = current->getNext();
        delete temp;
    }
    head_ = nullptr;
    size_ = 0;
}

//assignment operator
template <typename T>
Collection<T>& Collection<T>::operator=(const Collection<T>& other) {
    if (this == &other) return *this; //self-assignment check
    
    this->~Collection(); //clean up current object

    if(!other.head_) return *this; //other is empty
    //copy first
    head_ = new node<T>;
    head_->setData(other.head_->getData());
    node<T> *current = head_;
    node<T> *otherCurrent = other.head_->getNext();
    //copy rest
    while (otherCurrent) {
        node<T> *newNode = new node<T>;
        newNode->setData(otherCurrent->getData());
        current->setNext(newNode);
        current = newNode;
        otherCurrent = otherCurrent->getNext();
    }
    size_ = other.size_;
    return *this;
}

//add element to back
template <typename T>
void Collection<T>::add(const T& element) {
    node<T> *newNode = new node<T>;
    newNode->setData(element);
    if (!head_) {
        head_ = newNode; //new node becomes head
    } else {
        //go to last
        node<T> *current = head_;
        while (current->getNext() != nullptr) {
            current = current->getNext();
        }
        current->setNext(newNode); //attatch new node
    }
    ++size_;
}

//remove all instances of element
template <typename T>
void Collection<T>::remove(const T& element) {
    node<T> *current = head_;
    node<T> *prev = nullptr;
    while (current) {
        if (current->getData() == element) {
            //if match, delete
            node<T> *temp = current;
            if (prev) {
                prev->setNext(current->getNext());
            } else {
                head_ = current->getNext();//remove head
            }
            current = current->getNext();
            delete temp;
            --size_;
        } else {
            //traverse forward
            prev = current;
            current = current->getNext();
        }
    }
}  
//return last element
template <typename T>
T Collection<T>::last() const {
    if (!head_) throw std::runtime_error("Collection is empty");
    node<T> *current = head_;
    while (current->getNext() != nullptr) {
        current = current->getNext();
    }
    return current->getData();
}

//print collection elements
template <typename T>
void Collection<T>::print() const {
    node<T> *current = head_;
    while (current) {
        std::cout << current->getData() << " ";
        current = current->getNext();
    }
    std::cout << std::endl;
    if(size_ >10) {
        std::cout << "large container " << std::endl;
    }
}

//equality operator
//specific for int
inline bool equal(const Collection<int>& c1, const Collection<int>& c2) {
    if (c1.size_ != c2.size_) return false;
    node<int> *current1 = c1.head_;
    node<int> *current2 = c2.head_;
    while (current1 && current2) {
        if (current1->getData() != current2->getData()) return false;
        current1 = current1->getNext();
        current2 = current2->getNext();
    }
    return true;
}
//specific for string
inline bool equal(const Collection<std::string>& c1, const Collection<std::string>& c2) {
    if (c1.size_ != c2.size_) return false;
    node<std::string> *current1 = c1.head_;
    node<std::string> *current2 = c2.head_;
    while (current1 && current2) {
        if (current1->getData() != current2->getData()) return false;
        current1 = current1->getNext();
        current2 = current2->getNext();
    }
    return true;
}
//specific for char
inline bool equal(const Collection<char>& c1, const Collection<char>& c2) {
    if (c1.size_ != c2.size_) return false;
    node<char> *current1 = c1.head_;
    node<char> *current2 = c2.head_;
    while (current1 && current2) {
        if (current1->getData() != current2->getData()) return false;
        current1 = current1->getNext();
        current2 = current2->getNext();
    }
    return true;
}
//specific for double
inline bool equal(const Collection<double>& c1, const Collection<double>& c2) {
    if (c1.size_ != c2.size_) return false;
    node<double> *current1 = c1.head_;
    node<double> *current2 = c2.head_;
    while (current1 && current2) {
        if (current1->getData() != current2->getData()) return false;
        current1 = current1->getNext();
        current2 = current2->getNext();
    }
    return true;
}
//specific for Collection<char>
inline bool equal(const Collection<Collection<char>>& c1, const Collection<Collection<char>>& c2) {
    if (c1.size_ != c2.size_) return false;
    node<Collection<char>> *current1 = c1.head_;
    node<Collection<char>> *current2 = c2.head_;
    while (current1 && current2) {
        if (!equal(current1->getData(), current2->getData())) return false;
        current1 = current1->getNext();
        current2 = current2->getNext();
    }
    return true;
}  

#endif // COLLECTION_HPP_

