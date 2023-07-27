#ifndef QUERYPROCESSOR_HPP
#define QUERYPROCESSOR_HPP

#include "indexhandler.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <chrono>

/**
 * QueryProcessor class
 * Authored by Alexandra Ingrando
 * Communicates between user and the index.
 */

class QueryProcessor{
private:
    IndexHandler index;
    void maitenceMode();
    void interactiveMode();
    void search(string);
    void printResults(vector<pair<string, int>> &);
public:
    QueryProcessor();
    /**
     * void menu() initializes the queryprocesser, triggering  the different menu modes and allows you to search the search engine
     */
    void menu();

};

#endif // QUERYPROCESSOR_HPP
