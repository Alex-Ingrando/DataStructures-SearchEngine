#ifndef ENTRYINTERFACE_H
#define ENTRYINTERFACE_H

#include <string>
#include <iostream>
#include <vector>

using std::string;
using std::vector;
using std::pair;


/**
 * EntryInterface Class
 * Authored by Michael Young
 * Base class of both inverted entry and document entry objects;
 */
class EntryInterface
{
private:
    string name;
    vector<pair<string, int>> freq;

    //used to find the location of a word in the freq vector
    int find(string a);

    //adds the document to the freq vector
    void addDoc(string& a);

public:
    //default constructor
    EntryInterface();
    /**
     *  EntryInterface(const string& word) creates the object and initializes name to word
     */
    EntryInterface(const string& word);
    /**
     * EntryInterface(const string& word, const string& doc) creates the object, initializes name to word, and adds the first document to frequency
     */
    EntryInterface(const string& word, const string& doc);
    /**
     * EntryInterface(const string& word, pair<string, int> location) creates object, initializes name to word, and adds the first document frequency pair
     */
    EntryInterface(const string& word, pair<string, int> location);
    /**
     * EntryInterface(const string& word, vector<pair<string, int>> location) creates object, initializes name to word, and sets vector of documents and frequency to location.
     */
    EntryInterface(const string& word, vector<pair<string, int>> location);

    //get functions
    /**
     * string& getName() returns the name of the object. If the object is an inverted entry, this is the word.
     * If the object is a document, it is the document Id;
     */
    string& getName();
    /**
     * vector<pair<string, int>>& getFreq() returns a vector of pairs. If the object is an inverted entry,
     * this pair is the id of documents the word appeared in paired to the number of times it appeared.
     */
    vector<pair<string, int>>& getFreq();

    //copy constructor
    EntryInterface(const EntryInterface& rhs);

    //set up or change the name
    /**
     * void rename(string&) allows you to change the name of the object.
     */
    void rename(string& a);

    //increments the frequency value of that document
    /**
     * void updateFreq(string&) adds new documents to the inverted entry and increases the number of times
     * the entry appeared in the document.
     */
    void updateFreq(string& a);
    /**
     * int getAmount() calculates and returns the number of times the word appears in all of the documents.
     */
    //finds the total number of times a word has appeared
    int getAmount();
    /**
     * int getSpecific(string& a) returns the number of times the word appears in the document id, passed as a parameter.
     */
    //finds the total number of times a word has appeared in a specific question
    int getSpecific(string& a);
    /**
     * string outputFreq() write the document id and the frequency into a file.
     */
    string outputFreq();


    //overload comparison operators
    /**
     * Operators '>' have been overloaded to compare the objects by their string name;
     */
    bool operator> (const EntryInterface& rhs)
    {   return name > rhs.name;  }
    /**
     * Operators '<' have been overloaded to compare the objects by their string name;
     */
    bool operator< (const EntryInterface& rhs)
    {   return name < rhs.name;  }
    /**
     * Operators '>=' have been overloaded to compare the objects by their string name;
     */
    bool operator>= (const EntryInterface& rhs)
    {   return name >= rhs.name;  }
    /**
     * Operators '<=' have been overloaded to compare the objects by their string name;
     */
    bool operator<= (const EntryInterface& rhs)
    {   return name <= rhs.name;  }
    /**
     * Operators '==' have been overloaded to compare the objects by their string name;
     */
    bool operator== (const EntryInterface& rhs)
    {   return name == rhs.name;  }

    /**
     * Operator '<<' prints out the string name of the object.
     */
    //friend os and ifstream
    friend std::ostream& operator<< (std::ostream &out, const EntryInterface &s)
    {
        out << s.name;
        return out;
    }

    //destructor
    ~EntryInterface();

};

#endif // ENTRYINTERFACE_H
