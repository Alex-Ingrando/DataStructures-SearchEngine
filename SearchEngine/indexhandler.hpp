#ifndef INDEXHANDLER_HPP
#define INDEXHANDLER_HPP

#include "documentparser.hpp"
#include "entryinterface.h"
#include <vector>
/**
 * IndexHandler Class
 * Authored by Alexandra Ingrando
 * Communicates between the query processor and the document parser.
 */
class IndexHandler{
private:
    bool indexExist;
    bool indexTree;
    bool indexTable;
    bool persistedTree;
    bool persistedTable;

    DocumentParser parser;

    AVLIndex<EntryInterface> treeIndex;
    HashIndex<string, EntryInterface> tableIndex;
    vector<EntryInterface> documentIndex;
    EntryInterface searchTree(string);
    EntryInterface searchTable(string);

public:
    IndexHandler();
    /**
     * string checkPersisted() returns the string from DocumentParser.checkPersisted(), and tells the IndexHandler
     * what type of index has been stored.
     */
    string checkPersisted();
    /**
     * void loadTree() has DocumentParser store paresed files in an AVL Tree
     */
    void loadTree();
    /**
     * void loadTable() has DocumentParser store paresed files in an HashTable
     */
    void loadTable();
    /**
     * EntryInterface getDocument(string) returns the document object that with string id.
     */
    EntryInterface getDocument(string);
    /**
     * indexExists() returns a bool of whether the index has been loaded in an AVL Tree or a Hash Table
     */
    bool indexExists();
    /**
     * indexAVLTree() returns a bool of whether the index exists in an AVL Tree
     */
    bool indexAVLTree();
    /**
     * indexHashTable() returns a bool of whether the index exists in a Hash Table
     */
    bool indexHashTable();
    /**
     * void getStatistics() calls the DocumentParser.getStatistics()
     */
    void getStatistics();
    /**
     * void writeIndex() determines whether the AVL tree index or Hash table index should be written, and communicates this
     * to the DocumentParser.
     */
    void writeIndex();
    /**
     * EntryInterface search(string) returns the object from either the AVL tree index or Hash table index that matches the string.
     */
    EntryInterface search(string);
    /**
     * void deleteIndex() clears the current Index and resets all bools that indicate an index exists.
     */
    void deleteIndex();

};
#endif // INDEXHANDLER_HPP
