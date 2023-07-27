#include "entryinterface.h"

using std::string;

//default constructors
EntryInterface::EntryInterface()
{

}

EntryInterface::EntryInterface(const string& word)
{
    name = word;
}

EntryInterface::EntryInterface(const string &word, const std::string &doc)
{
    name = word;
    string document = doc;
    this->addDoc(document);
}

EntryInterface::EntryInterface(const string& word, pair<string, int> location)
{
    name = word;
    freq.push_back(location);
}

EntryInterface::EntryInterface(const string& word, vector<pair<string, int>> location)
{
    name = word;
    freq = location;
}

//finds the location of the passed document on the freq vector
int EntryInterface::find(string a)
{
    //goes through the freq vector to find a match
    int size = freq.size();
    //for(int i=freq.size() - 1; i>=0; i--)
    //{
        //returns the location number once it has been found
        if(freq[size - 1].first==a)
        {   return size- 1;   }
    //}

    //if the file is not in the vector
    return -1;
}

//creates a new pair instance within the vector
void EntryInterface::addDoc(string& doc)
{
    pair<string, int> instance(doc, 1);
    freq.push_back(instance);
}


//get functions
string& EntryInterface::getName()
{
    return name;
}

string EntryInterface::outputFreq()
{
    string freqOutput;
    for(unsigned int i=0; i<getFreq().size(); i++)
    {
        freqOutput += (getFreq()[i].first + "-" + std::to_string(getFreq()[i].second)) + "|";
    }

    return freqOutput;
}

vector<pair<string, int>>& EntryInterface::getFreq()
{
    return freq;
}


//copy constructor
EntryInterface::EntryInterface(const EntryInterface& rhs)
{
    name = rhs.name;
    freq = rhs.freq;
}

void EntryInterface::rename(string& a)
{
    name = a;
}


void EntryInterface::updateFreq(string& a)
{
    int finder = find(a);

    //increments the value by 1
    if(finder>=0)
    {
        freq[finder].second = (freq[finder].second + 1);
        return;
    }

    //if it is not in the vector already, adds a new one
    this->addDoc(a);
}

int EntryInterface::getAmount()
{
    int total=0;
    for(unsigned int i=0; i<freq.size(); i++)
    {   total += freq[i].second;    }

    return total;
}

int EntryInterface::getSpecific(string& a)
{
    int x = find(a);
    return freq[x].second;
}


//destructor
EntryInterface::~EntryInterface()
{
    //freq.clear();
}
