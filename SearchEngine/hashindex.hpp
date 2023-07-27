#ifndef HASHINDEX_HPP
#define HASHINDEX_HPP

#include <string>
#include <vector>
#include <iostream>

#include "entryinterface.h"

using std::vector;
using std::cout;
using std::endl;


/**
 * HashIndex class
 * Authored by Michael
 * HashIndex stores a templated object with a templated key into a hash table.
 */
template <class K, class V>
class HashIndex
{
private:

    vector<V> data;
    vector<bool> used;
    int size;
    V* lastSearched;

public:
    //rule of three
    HashIndex();
    HashIndex(const HashIndex&);
    ~HashIndex();

    /**
     * vector<V> getData() returns the hash table as a vector.
     */
    vector<V> getData()
    {   return data;    }
    /**
     * int getSize() returns the number of elements stored in the hash table.
     */
    int getSize()
    {   return size;    }

    /**
     * void add(k, V&) accepts a k type key and an object that is hashed and stored in the table.
     */
    void add(K, V&);
    /**
     * addFromInput(int, V&) adds a passed object at the position int.
     */
    void addFromInput(int, V&);
    /**
     * bool inHash(K) returns whether or not an object with the key K is currently stored in the tree.
     */
    bool inHash(K);
    /**
     * V* search(K) finds the V object that matches the K key, and returns a pointer to the object.
     * Reccomended to use inHash(K) first.
     */
    V* search(K);
    /**
     * V& find(K) finds the V object that matches the K key, and returns a reference of the object.
     * Reccomended to use inHash(K) first.
     */
    V& find(K);
    /**
     * void deleteIndex() empties the Hash Table.
     */
    void deleteIndex();

};

template <class K, class V>
HashIndex<K,V>::HashIndex()
{
    data.resize(10000000);
    used.resize(10000000);
    used = {false};
    size=0;
}

template <class K, class V>
HashIndex<K,V>::HashIndex(const HashIndex& newData)
{
    this->data = newData.getData();
}

template <class K, class V>
void HashIndex<K, V>::add(K word, V& newData)
{
    //if false, the object must be added
    std::hash<K> hasher;
    int x = hasher(word)%10000000;

    data[x] = newData;
    //V* ptr = &data[x];

    if(used[x]==false)
    {
        used[x]=true;
        size++;
    }
}

template <class K, class V>
void HashIndex<K, V>::addFromInput(int x, V& newData)
{
    data[x] = newData;

    if(used[x]==false)
    {
        used[x]=true;
        size++;
    }
}

template <class K, class V>
bool HashIndex<K,V>::inHash(K word){
    std::hash<K> hasher;
    int x = hasher(word)%10000000;

    if(data[x] == word)
    {
        lastSearched = &data[x];
        return true;
    }


    return false;
}

template <class K, class V>
V* HashIndex<K,V>::search(K word)
{
    if(*lastSearched == word){
        return lastSearched;
    } else {
        std::hash<K> hasher;
        int x = hasher(word)%10000000;
        return &data[x];
    }

}

template <class K, class V>
V& HashIndex<K,V>::find(K word)
{
    std::hash<K> hasher;
    int x = hasher(word)%10000000;

    if(data[x] == word)
    {
        return data[x];
    }

    cout << "That word was not found in the index" <<endl;
}


template <class K, class V>
void HashIndex<K,V>::deleteIndex()
{
    data.clear();
    used.clear();
    size=0;
}

template <class K, class V>
HashIndex<K,V>::~HashIndex()
{
    data.clear();
    used.clear();
    size=0;
}

#endif // HASHINDEX_HPP
