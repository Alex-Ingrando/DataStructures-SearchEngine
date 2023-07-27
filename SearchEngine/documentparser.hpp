#ifndef DOCUMENTPARSER_HPP
#define DOCUMENTPARSER_HPP
#include "entryinterface.h"
#include "avlindex.hpp"
#include "hashindex.hpp"
#include "porter2_stemmer.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <random>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <dirent.h>
#include <chrono>


using namespace std;

/**
 * DocumentParser Class
 * Authored by Michael Young
 * DocumentParser reads and parses stack overflow tag and question .psv files.
 */
class DocumentParser{
private:
    char* argv[8];
    int argc;
    int questionsTotal;
    pair<int, string> mostFreq[50];

    bool static isUnwanted(char);
    vector<string>& tagfiler(int, vector<string>&, vector<string>& );
    void parseDocument(int, bool*, vector<string> , HashIndex<string, EntryInterface>&, vector<EntryInterface>&,
                       AVLIndex<EntryInterface>&, bool);
public:
    DocumentParser();
    /**
     * void loadFileNames() loads all of the files that will be parsed.
     */
    void loadFileNames();
    /**
     * void runParser(string, HashIndex<string, EntryInterface>&, vector<EntryInterface>&, AVLIndex<EntryInterface>&)
     * accepts a string to determine which index parsed words should be stored in, as well as the two possible index
     * objects and a vector to hold the question objects.
     */
    void runParser(string, HashIndex<string, EntryInterface>&, vector<EntryInterface>&, AVLIndex<EntryInterface>&);
    /**
     * string checkPersisted(int) checks to see if file has either an AVL tree or Hash Table stored in it,
     * and returns "AVL Tree" or "Hash Table" if stored; "No" is file is empty;
     */
    string checkPersisted(int);

    /**
     * void writeIndex(bool, HashIndex<string, EntryInterface>&, AVLIndex<EntryInterface>&, int) uses a bool true to write
     * AVL index, false to write a Hash Table index into file determined by the int.
     */
    void writeIndex(bool, HashIndex<string, EntryInterface>&, AVLIndex<EntryInterface>&, int);
    /**
     * void readIndex(bool, HashIndex<string, EntryInterface>&, AVLIndex<EntryInterface>&, int) uses a bool true to read
     * AVL index, false to write a Hash Table index into file determined by the int.
     */
    void readIndex(bool, HashIndex<string, EntryInterface>&, AVLIndex<EntryInterface>&, int);
    /**
     * void mostFrequentAVL(AVLIndex<EntryInterface>&) calculates the top 50 most frequent words in the tree, and stores
     * it in the global freq array.
     */
    void mostFrequentAVL(AVLIndex<EntryInterface>&);
    /**
     * void mostFrequentHash(HashIndex<string, EntryInterface>&) calculates the top 50 most frequent words in the table,
     * and stores it in the global freq array.
     */
    void mostFrequentHash(HashIndex<string, EntryInterface>&);
    /**
     * void outputAVL(AVLIndex<EntryInterface>&, int) writes the AVL tree into a file in preorder.
     */
    void outputAVL(AVLIndex<EntryInterface>&, int);
    /**
     * void outputAVL(AVLIndex<EntryInterface>&, int) writes the hash table into a file, stored with its key;
     */
    void outputHash(HashIndex<string, EntryInterface>&, int);
    /**
     * void inputAVL(AVLIndex<EntryInterface>&, int) reads the AVL tree from a file and stores it.
     */
    void inputAVL(AVLIndex<EntryInterface>&, int);
    /**
     * void inputHash(HashIndex<string, EntryInterface>&, int) reads the Hash table from a file and stores it.
     */
    void inputHash(HashIndex<string, EntryInterface>&, int);
    /**
     * getStatistics(bool, AVLIndex<EntryInterface>&, HashIndex<string, EntryInterface>&) prints statistics of the
     * index on the console. Passing true analyzes the AVL tree. Passing false analyzes the HashTable
     */
    void getStatistics(bool, AVLIndex<EntryInterface>&, HashIndex<string, EntryInterface>&);
    void parseWithoutReading(int, vector<string>, vector<EntryInterface>&);

};

#endif // DOCUMENTPARSER_HPP
