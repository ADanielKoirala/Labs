// implementation basic hashmap (unordered container)
// Wayne Cheng: adapted from Proc C++
// 4/10/2018

// modified by Daniel Koirala for CSIII
#ifndef HASHMAP_H
#define HASHMAP_H

#include <cstddef>
#include <utility>
#include <functional>
#include <vector>
#include <list>
#include <iterator>

using std::vector;
using std::list;
using std::pair;
using std::make_pair;

//////////////////////////////////////////
// hash function implemented as a class
//////////////////////////////////////////

// any Hash Class must provide
// two methods: hash() and numBuckets().
template <typename T>
class DefaultHash {
public:
    DefaultHash(size_t numBuckets = defaultNumBuckets);
    size_t hash(const T& key) const;
    size_t numBuckets() const { return numBuckets_; }

private:
    static const size_t defaultNumBuckets = 101; // default number of buckets in the hash
    size_t numBuckets_;
};

template <typename T>
DefaultHash<T>::DefaultHash(size_t numBuckets)
    : numBuckets_(numBuckets > 0 ? numBuckets : defaultNumBuckets) {}

// uses the division method for hashing.
// treats the key as a sequence of bytes, sums the ASCII
// values of the bytes, and mods the total by the number
// of buckets.
template <typename T>
size_t DefaultHash<T>::hash(const T& key) const {
    size_t res = 0;
    for (size_t i = 0; i < sizeof(key); ++i) {
        const unsigned char b =
            *(reinterpret_cast<const unsigned char*>(&key) + i);
        res += b;
    }
    return res % numBuckets_;
}

////////////////////////////////////////////////
// container class
////////////////////////////////////////////////

template <typename Key, typename Value,
          typename Compare = std::equal_to<Key>,
          typename Hash = DefaultHash<Key>>
class hashmap {

public:
    typedef pair<const Key, Value> Element;

    // constructor
    // invokes constructors for comparison and hash objects
    hashmap(const Compare& comp = Compare(),
            const Hash& hash = Hash());

    // destructor, copy constructor, move constructor,
    // copy assignment operator and move assignment operator
    ~hashmap();
    hashmap(const hashmap<Key, Value, Compare, Hash>& src);
    hashmap(hashmap<Key, Value, Compare, Hash>&& src); // C++11

    hashmap<Key, Value, Compare, Hash>& operator=(
        hashmap<Key, Value, Compare, Hash> rhs);
    hashmap<Key, Value, Compare, Hash>& operator=(
        hashmap<Key, Value, Compare, Hash>&& rhs); // C++11

    // swap with usual semantics
    void swap(hashmap<Key, Value, Compare, Hash>& rhs);

    // inserts the key/value pair x (SAFE INSERT)
    // returns <pointer, inserted?>
    // pointer -> existing element with same key OR newly inserted element
    pair<Element*, bool> insert(const Element& x);

    // removes the Element with key x, if it exists (NEW ERASE)
    // returns <pointer_to_element_after_erased, erased?>
    pair<Element*, bool> erase(const Key& x);

    // returns a pointer to the element with key x
    // returns nullptr if no element with this key
    Element* find(const Key& x);

    // finds the element with key x, inserts an
    // element with that key if none exists yet. Returns a reference to
    // the value corresponding to that key.
    Value& operator[](const Key& x);

    // change number of buckets (only grows)
    void rehash(size_t n);

    size_t size() const { return size_; }
    size_t bucket_count() const { return hash_.numBuckets(); }

private:
    // helper function for various searches
    typename list<Element>::iterator
    findElement(const Key& x,
                const size_t bucket);

    // helper: pointer to element *after* a given position (possibly in later bucket)
    Element* nextElementPtrAfter(
        size_t bucket,
        typename list<Element>::iterator it_after_erased);

    size_t size_;
    Compare comp_;
    Hash hash_;

    // hash contents: vector buckets
    // each bucket is a list containing key->value pairs
    vector<list<Element>> elems_;
};

//
// constructors/destructors
//

// Construct elems_ with the number of buckets.
template <typename Key, typename Value, typename Compare, typename Hash>
hashmap<Key, Value, Compare, Hash>::hashmap(
    const Compare& comp, const Hash& hash)
    : size_(0), comp_(comp), hash_(hash) {
    elems_ = vector<list<Element>>(hash_.numBuckets());
}

template <typename Key, typename Value, typename Compare, typename Hash>
hashmap<Key, Value, Compare, Hash>::~hashmap() {}

template <typename Key, typename Value, typename Compare, typename Hash>
hashmap<Key, Value, Compare, Hash>::hashmap(
    const hashmap<Key, Value, Compare, Hash>& src)
    : size_(src.size_), comp_(src.comp_),
      hash_(src.hash_), elems_(src.elems_) {}

// swap function
template <typename Key, typename Value, typename Compare, typename Hash>
void hashmap<Key, Value, Compare, Hash>::swap(
    hashmap<Key, Value, Compare, Hash>& rhs) {
    std::swap(elems_, rhs.elems_);
    std::swap(size_, rhs.size_);
    std::swap(comp_, rhs.comp_);
    std::swap(hash_, rhs.hash_);
}

// C++11 move constructor
template <typename Key, typename Value, typename Compare, typename Hash>
hashmap<Key, Value, Compare, Hash>::hashmap(
    hashmap<Key, Value, Compare, Hash>&& src) {
    elems_ = std::move(src.elems_);
    size_ = src.size_;
    src.size_ = 0;
    comp_ = src.comp_;
    hash_ = src.hash_;
}

// overloaded assignment, copy-and-swap implementation
template <typename Key, typename Value, typename Compare, typename Hash>
hashmap<Key, Value, Compare, Hash>&
hashmap<Key, Value, Compare, Hash>::operator=(
    hashmap<Key, Value, Compare, Hash> rhs) {
    swap(rhs);
    return *this;
}

// C++11 move assignment operator
template <typename Key, typename Value, typename Compare, typename Hash>
hashmap<Key, Value, Compare, Hash>&
hashmap<Key, Value, Compare, Hash>::operator=(
    hashmap<Key, Value, Compare, Hash>&& rhs) {
    // check for self-assignment
    if (this != &rhs) {
        // move ownership
        elems_ = std::move(rhs.elems_);
        size_ = rhs.size_;
        rhs.size_ = 0;
        comp_ = rhs.comp_;
        hash_ = rhs.hash_;
    }
    return *this;
}

//
// rest of container member functions
//

// helper function
template <typename Key, typename Value,
          typename Compare, typename Hash>
typename list<pair<const Key, Value>>::iterator
hashmap<Key, Value, Compare, Hash>::findElement(
    const Key& x, size_t bucket) {

    // look for the key in the bucket
    for (auto it = elems_[bucket].begin();
         it != elems_[bucket].end(); ++it)
        if (comp_(it->first, x))
            return it;

    return elems_[bucket].end(); // element not found
}

template <typename Key, typename Value, typename Compare, typename Hash>
typename hashmap<Key, Value, Compare, Hash>::Element*
hashmap<Key, Value, Compare, Hash>::find(const Key& x) {
    size_t bucket = hash_.hash(x);
    auto it = findElement(x, bucket); // use the findElement() helper

    if (it == elems_[bucket].end())
        // we didn't find the element -- return nullptr
        return nullptr;

    // we found the element. Return a pointer to it.
    return &(*it); // dereference the iterator to list
                   // then take the address of list element
}

// SAFE INSERT
// returns <ptr_to_existing_or_new_element, inserted?>
template <typename Key, typename Value, typename Compare, typename Hash>
pair<typename hashmap<Key, Value, Compare, Hash>::Element*, bool>
hashmap<Key, Value, Compare, Hash>::insert(const Element& x) {
    size_t bucket = hash_.hash(x.first);
    auto it = findElement(x.first, bucket); // see if already present

    if (it != elems_[bucket].end()) {
        // already present -- do not insert
        return { &(*it), false };
    }

    elems_[bucket].push_back(x); // insert new element
    ++size_;
    auto last = elems_[bucket].end();
    --last; // iterator to newly inserted element
    return { &(*last), true };
}

// operator[] using safe insert; no second find()
template <typename Key, typename Value, typename Compare, typename Hash>
Value& hashmap<Key, Value, Compare, Hash>::operator[](const Key& x) {
    // try fast path first: find existing
    Element* found = find(x);
    if (found == nullptr) {
        // not found: insert default-constructed value
        auto res = insert(make_pair(x, Value()));
        found = res.first; // pointer from safe insert
    }
    return found->second;
}

// ERASE returning <ptr_to_next_element, erased?>
template <typename Key, typename Value, typename Compare, typename Hash>
pair<typename hashmap<Key, Value, Compare, Hash>::Element*, bool>
hashmap<Key, Value, Compare, Hash>::erase(const Key& x) {
    size_t bucket = hash_.hash(x);
    auto it = findElement(x, bucket);

    if (it == elems_[bucket].end()) {
        // element not found: result = false,
        // pointer is unspecified, we'll just return nullptr
        return { nullptr, false };
    }

    auto it_next = std::next(it);
    Element* nextPtr = nextElementPtrAfter(bucket, it_next);

    // erase current
    elems_[bucket].erase(it);
    --size_;
    return { nextPtr, true };
}

// helper to compute "element after erased", possibly in later buckets
template <typename Key, typename Value, typename Compare, typename Hash>
typename hashmap<Key, Value, Compare, Hash>::Element*
hashmap<Key, Value, Compare, Hash>::nextElementPtrAfter(
    size_t bucket,
    typename list<Element>::iterator it_after_erased) {

    // 1) same bucket, if there is a next element
    if (it_after_erased != elems_[bucket].end()) {
        return &(*it_after_erased);
    }

    // 2) search subsequent buckets for the first non-empty one
    for (size_t b = bucket + 1; b < elems_.size(); ++b) {
        if (!elems_[b].empty()) {
            return &elems_[b].front();
        }
    }

    // 3) nothing after erased element
    return nullptr;
}

// rehash, if n <= current buckets, do nothing. Otherwise expand and redistribute.
template <typename Key, typename Value, typename Compare, typename Hash>
void hashmap<Key, Value, Compare, Hash>::rehash(size_t n) {
    if (n <= elems_.size()) return;

    vector<list<Element>> newElems(n);
    Hash newHash(n); // new hash object with updated bucket count

    for (size_t b = 0; b < elems_.size(); ++b) {
        for (auto& e : elems_[b]) {
            size_t newB = newHash.hash(e.first);
            newElems[newB].push_back(e);
        }
    }

    elems_.swap(newElems);
    hash_ = newHash; // update hash functor to know new bucket count
    // size_ remains unchanged (same number of elements)
}

#endif // HASHMAP_H
