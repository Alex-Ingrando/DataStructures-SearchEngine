#include "documentparser.hpp"
using namespace std::chrono;
using namespace std;

DocumentParser::DocumentParser(){

    questionsTotal = 0;
}

void DocumentParser::loadFileNames(){
    this->argv[0] = "../files/output.txt";
    this->argv[1] = "../files/stopwords.psv";
    this->argv[2] = "../files/2008-tags.psv";
    this->argv[3] = "../files/2008-questions.psv";
    this->argv[4] = "../files/2009-tags.psv";
    this->argv[5] = "../files/2009-questions.psv";
    this->argv[6] = "../files/2010-tags.psv";
    this->argv[7] = "../files/2010-questions.psv";
    this->argv[8] = "../files/2011-tags.psv";
    this->argv[9] = "../files/2011-questions.psv";
    this->argv[10] = "../files/2012-tags.psv";
    this->argv[11] = "../files/2012-questions.psv";
    this->argv[12] = "../files/2013-tags.psv";
    this->argv[13] = "../files/2013-questions.psv";
    this->argv[14] = "../files/2014-tags.psv";
    this->argv[15] = "../files/2014-questions.psv";
    this->argv[16] = "../files/2015-tags.psv";
    this->argv[17] = "../files/2015-questions.psv";
    this->argv[18] = "../files/2016-tags.psv";
    this->argv[19] = "../files/2016-questions.psv";
    this->argc = 20;
    questionsTotal = 0;
}

bool DocumentParser::isUnwanted(char x)
{
    //only allows lowercase alpha and + #
    return ((int(x) < 35) || (int(x) > 35 && int(x) < 43) || (int(x) > 43 && int(x) < 97) || (int(x) > 122));
}


vector<string>& DocumentParser::tagfiler(int v, vector<string>& tagVector, vector<string>& tagFields)
{
    string tagger;
    string tagWord;

    int IDdivide;

    ifstream tagfile;
    tagfile.open(argv[v], ios::in);

    //first line is useless, discard it
    tagfile >> tagger;

    //this is the first iteration, it must complete this once before it enters the while loop
    tagfile >> tagger;
    IDdivide = tagger.find('|');
    tagWord = tagger.substr((IDdivide+1), tagger.size());
    tagger = tagger.substr(0, IDdivide);

    tagVector.push_back(tagger);
    tagFields.push_back(tagWord);


    while(!tagfile.eof())
    {
        tagfile >> tagger;
        IDdivide = tagger.find('|');
        tagWord = tagger.substr((IDdivide+1), tagger.size());
        tagger = tagger.substr(0, IDdivide);

        if(tagger==tagVector.back())
        {
            continue;
        }

        tagVector.push_back(tagger);
        tagFields.push_back(tagWord);
    }

    tagfile.close();
    return tagVector;
}



void DocumentParser::parseDocument(int v, bool* stopWords, vector<string> tagVector, HashIndex<string, EntryInterface>& hashIndex,
                   vector<EntryInterface>& documentIndex, AVLIndex<EntryInterface>& AVLindex, bool indexMethod)
{
    ifstream readfile;
    readfile.open(argv[v], ios::in);

    string reader;
    std::hash<string> hasher;
    EntryInterface* entryPtr;

    int IDdivide;
    string possibleID;
    string userID;
    string postDate;

    int foundQuestions=0;
    bool inTitle = true;
    int currentID = stoi(tagVector[0]);

    //first line is disposable, ignore it
    readfile >> reader;

    while(!readfile.eof())
    {
        readfile >> reader;

        //checks if the word might be the next question
        if(isdigit(reader[0])==true)
        {
            //this gets the ID of the question
            IDdivide = reader.find('|');
            possibleID = reader.substr(0, IDdivide);

            if(possibleID==tagVector[foundQuestions])
            {
                reader.erase(0, IDdivide+1);

                //this gets the user ID of the question
                IDdivide = reader.find('|');
                userID = reader.substr(0, IDdivide);
                reader.erase(0, IDdivide+1);

                //this gets the Date of the Question
                IDdivide = reader.find('|');
                postDate = reader.substr(0, IDdivide);

                currentID = stoi(possibleID);
                //everything after the last | is the first word of the Title
                IDdivide = (reader.find_last_of('|') + 1);

                documentIndex[currentID].getName() = reader.substr(IDdivide);

                inTitle=true;
                foundQuestions++;
                continue;
            }

            IDdivide=0;
        }

        if(inTitle==true)
        {
            if(reader.find('|') == std::string::npos)
            {
                documentIndex[currentID].getName() += (" " + reader);
            }

            else
            {
                documentIndex[currentID].getName() += reader.substr(0, reader.find('|'));
                documentIndex[currentID].getName() += (" - Posted By: " + userID + " - Date Posted: " + postDate);
                //cout <<endl << documentIndex[currentID].getName() <<endl;

                pair<string, int> firstBody( reader.substr((reader.find('|')+ 0),reader.size()) , 0 );
                documentIndex[currentID].getFreq().push_back(firstBody);
                inTitle=false;
            }
        }


        else if(inTitle==false)
        {
            documentIndex[currentID].getFreq()[0].first += (" " + reader);
        }

        if(reader.size()<3 || reader.size()>26)
        {   continue;   }

        for(unsigned int k=0; k<reader.size(); k++)
        {
            reader[k] = tolower(reader[k]);
        }

        reader.erase(std::remove_if(reader.begin(), reader.end(), isUnwanted), reader.end());


        //if its a stop word, ignore it
        if(stopWords[(hasher(reader) % 1000000)] == true)
        {
            continue;
        }

        Porter2Stemmer::stem(reader);

        if(reader != "" && foundQuestions < tagVector.size())
        {
            EntryInterface newEntry(reader, tagVector[foundQuestions]);
            entryPtr = &newEntry;

            if(indexMethod==true)
            {
                bool stored = AVLindex.inTree(newEntry);

                if(stored == true){
                    EntryInterface* ptr = AVLindex.find(newEntry);
                    ptr->updateFreq(tagVector[foundQuestions]);
                }

                else
                {
                    newEntry.updateFreq(tagVector[foundQuestions]);
                    AVLindex.add(newEntry);
                }
            }

           else
           {
               if(hashIndex.inHash(reader) == true){
                   EntryInterface* ptr = hashIndex.search(reader);
                   ptr->updateFreq(tagVector[foundQuestions]);
               } else {
                   hashIndex.add(reader, newEntry);
               }
           }
        }
    }

    readfile.close();
    questionsTotal+= foundQuestions;
}

void DocumentParser::parseWithoutReading(int v, vector<string> tagVector, vector<EntryInterface>& documentIndex)
{
    ifstream readfile;
    readfile.open(argv[v], ios::in);

    string reader;

    int IDdivide;
    string possibleID;
    string userID;
    string postDate;

    int foundQuestions=0;
    bool inTitle = true;
    int currentID = stoi(tagVector[0]);

    //first line is disposable, ignore it
    readfile >> reader;

    while(!readfile.eof())
    {
        readfile >> reader;

        //checks if the word might be the next question
        if(isdigit(reader[0])==true)
        {
            //this gets the ID of the question
            IDdivide = reader.find('|');
            possibleID = reader.substr(0, IDdivide);

            if(possibleID==tagVector[foundQuestions])
            {
                reader.erase(0, IDdivide+1);

                //this gets the user ID of the question
                IDdivide = reader.find('|');
                userID = reader.substr(0, IDdivide);
                reader.erase(0, IDdivide+1);

                //this gets the Date of the Question
                IDdivide = reader.find('|');
                postDate = reader.substr(0, IDdivide);

                currentID = stoi(possibleID);
                //everything after the last | is the first word of the Title
                IDdivide = (reader.find_last_of('|') + 1);

                documentIndex[currentID].getName() = reader.substr(IDdivide);

                inTitle=true;
                foundQuestions++;
                continue;
            }

            IDdivide=0;
        }

        if(inTitle==true)
        {
            if(reader.find('|') == std::string::npos)
            {
                documentIndex[currentID].getName() += (" " + reader);
            }

            else
            {
                documentIndex[currentID].getName() += reader.substr(0, reader.find('|'));
                documentIndex[currentID].getName() += (" - Posted By: " + userID + " - Date Posted: " + postDate);

                pair<string, int> firstBody( reader.substr((reader.find('|')+ 0),reader.size()) , 0 );
                documentIndex[currentID].getFreq().push_back(firstBody);
                inTitle=false;
            }
        }


        else if(inTitle==false)
        {
            documentIndex[currentID].getFreq()[0].first += (" " + reader);
        }

    }

    readfile.close();
    questionsTotal+= foundQuestions;
}


void DocumentParser::runParser(string inputCommand, HashIndex<string, EntryInterface>& hashIndex,
                               vector<EntryInterface>& documentIndex, AVLIndex<EntryInterface>& AVLindex)
{
    bool indexMethod = false;

        if(inputCommand=="1")
        {   indexMethod=true;   }

        //used to remove stopwords
        bool stopWords[1000000] = {false};
        documentIndex.resize(34958461);

        ifstream readfile;

        string reader;
        hash<string> hasher;
        readfile.open(argv[1], ios::in);
        vector<string> tagVector;

        //for each tag in tagVector, tagFields will hold the list of tags that map to them
        vector<string> tagFields;

        //stores all the stopwords
        while(!readfile.eof())
        {

            readfile >> reader;

            //sets stopword location to true to trigger stopword removal
            stopWords[(hasher(reader) % 1000000)] = true;
        }

        //iterates over each file
        for(int i=2; i<argc; i++)
        {
            tagfiler(i, tagVector, tagFields);
            i++;
            if(inputCommand == "Persisted"){
                parseWithoutReading(i, tagVector, documentIndex);
            } else{
               parseDocument( i, stopWords, tagVector, hashIndex, documentIndex, AVLindex, indexMethod);
            }

            tagVector.clear();
        }

}



string DocumentParser::checkPersisted(int v)
{
    ifstream readfile;
    readfile.open(argv[v], ios::in);
    string reader;
    getline(readfile, reader);

    readfile.close();
    if(reader.empty())
    {   return "No";    }

    return reader;
}


void DocumentParser::writeIndex(bool indexMethod, HashIndex<string, EntryInterface>& hashIndex, AVLIndex<EntryInterface>& AVLindex, int v)
{
    if(indexMethod==true)
    {
        outputAVL(AVLindex, v);
    }

    else
    {
        outputHash(hashIndex, v);
    }
}

void DocumentParser::readIndex(bool indexMethod, HashIndex<string, EntryInterface>& hashIndex, AVLIndex<EntryInterface>& AVLindex, int v)
{
    if(indexMethod==true)
    {
        inputAVL(AVLindex, v);
    }

    else
    {
        outputHash(hashIndex, v);
    }
}

void DocumentParser::mostFrequentAVL(AVLIndex<EntryInterface>& AVLindex)
{
    int x = 0;
    bool full = false;
    vector<EntryInterface> tree = AVLindex.preOrder();
    cout << tree.size() << endl;
    for(int i = 0; i < tree.size(); i++){
        if(full){
            x = tree.at(i).getAmount();
            if(mostFreq[0].first>=x){
                continue;
            }
            for(int j = 1; j < 50; j++){
                if(mostFreq[j].first > x){
                    mostFreq[0].first = x;
                    mostFreq[0].second = tree.at(i).getName();
                    std::sort(mostFreq, mostFreq+49);
                }
            }
        } else
        {
            mostFreq[x].first = tree.at(i).getAmount();
            mostFreq[x].second = tree.at(i).getName();
            x++;
            if(x==50)
            {
                full = true;
                std::sort(mostFreq, mostFreq+49);
            }
        }
    }
}


void DocumentParser::mostFrequentHash(HashIndex<string, EntryInterface>& hashIndex)
{
    int x =0;
    bool full=false;
    for(int i=0; i<10000000; i++)
    {
        if(full)
        {
            x = hashIndex.getData()[i].getAmount();

            //if the smallest in the array is larger, skip this word
            if(mostFreq[0].first>=x)
            {   continue;   }

            for(int j=1; j<50; j++)
            {
                if(mostFreq[j].first>x)
                {
                    mostFreq[0].first = x;
                    mostFreq[0].second = hashIndex.getData()[i].getName();
                    cout << "changed" << endl;
                    std::sort(mostFreq, mostFreq+49);
                }
            }
        }

        else
        {
            auto start = high_resolution_clock::now();
            mostFreq[x].first = hashIndex.getData()[i].getAmount();
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start) ;
            cout << "Time taken to read files and load index: "
                     << duration.count() / 1000000.00 << " seconds" << endl;
            start = high_resolution_clock::now();
            mostFreq[x].second = hashIndex.getData()[i].getName();
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start) ;
            cout << "Time taken to read files and load index: "
                     << duration.count() / 1000000.00 << " seconds" << endl;
            x++;
            cout << "add" << endl;
            if(x==50)
            {
                full = true;
                std::sort(mostFreq, mostFreq+49);
            }
        }
    }
}


void DocumentParser::outputAVL(AVLIndex<EntryInterface>& AVLindex, int v)
{
    cout << "Writing current AVL index to file ..." << endl;
    std::ofstream outputFile(argv[v]);
    vector<EntryInterface> output = AVLindex.preOrder();
    outputFile << "AVL Tree" << endl;
    outputFile << questionsTotal << endl;
    outputFile << output.size()<<endl;
    outputFile << "///////////" <<endl;

    for(int i = 0; i < output.size(); i++){
        outputFile << i << "|" << output.at(i).getName() << "|" << output.at(i).outputFreq() << endl;
    }
    outputFile.close();
    cout << "Index stored. Thank you for using the Stack Overflow search engine." << endl;
}


void DocumentParser::outputHash(HashIndex<string, EntryInterface>& hashIndex, int v)
{
    cout << "Writing current Hash Table to file ..." << endl;
    std::ofstream outputFile(argv[v]);
    EntryInterface temp;
    outputFile << "Hash Table" << endl;
    outputFile << questionsTotal << endl;
    outputFile << hashIndex.getSize() <<endl;

    outputFile << "///////////" <<endl;

    for(int i=0; i<10000000; i++)
    {
        temp = hashIndex.getData()[i];
        outputFile << i << "|" << temp.getName() << "|" << temp.outputFreq() <<endl;
        //it will look something like this:
        // 23946|comput|90-1|95-2|340-7|5560-70
    }

    outputFile.close();
    cout << "Index stored. Thank you for using the Stack Overflow search engine." << endl;
}


void DocumentParser::inputAVL(AVLIndex<EntryInterface>& AVLindex, int v)
{
    ifstream readfile;
    readfile.open(argv[v], ios::in);
    string reader;
    int divider1=0;
    int divider2=0;
    int finaldivide;

    EntryInterface temp;
    int ID;
    string word;
    string questionID;
    int questionFreq;
    vector<pair<string, int>> question;

    readfile >> reader; //should read "HashTable or AVL
    readfile >> reader; //should read questionsTotal
    questionsTotal = stoi(reader);
    readfile >> reader; //should read number of elements
    int wordNum = stoi(reader);

    readfile >> reader; //should read "///////////"

    //from here on contains the rest of the stored words
    while(!readfile.eof())
    {
        readfile >> reader;

        //the first section is the location on the HashTable
        //divider1 should = 0
        divider2 = reader.find('|');
        ID = stoi(reader.substr(divider1, divider2));
        divider1 = (divider2+1);
        //this replaces the deliminator with another that is not used so the next "find('|')" will skip the '~'
        reader.replace(divider2, divider1, "~");


        //the second section is the actual word itself
        divider2 = reader.find('|');
        word = reader.substr(divider1, divider2);
        divider1 = (divider2+1);
        reader.replace(divider2, divider1, "~");

        //this finds where the last place the divide is
        finaldivide = reader.find_last_of('|');

        //adds to the vector until it hits the end
        while(divider2>=finaldivide)
        {
            divider2 = reader.find('-');
            questionID = reader.substr(divider1, divider2);
            divider1 = (divider2+1);
            reader.replace(divider2, divider1, "~");

            divider2 = reader.find('|');
            questionFreq = stoi( reader.substr(divider1, divider2) );
            divider1 = (divider2+1);
            reader.replace(divider2, divider1, "~");

            pair<string, int> instance(questionID, questionFreq);
            question.push_back(instance);
        }

        //completes the final instance
        divider2 = reader.find('-');
        questionID = reader.substr(divider1, divider2);
        divider1 = (divider2+1);
        reader.replace(divider2, divider1, "~");

        questionFreq = stoi( reader.substr(divider1) );
        divider1 = (divider2+1);

        pair<string, int> instance(questionID, questionFreq);
        question.push_back(instance);

        EntryInterface newEntry(word, question);
        AVLindex.add(newEntry);

        divider1=0;
        divider2=0;
    }

    readfile.close();
}



void DocumentParser::inputHash(HashIndex<string, EntryInterface>& hashIndex, int v)
{
    ifstream readfile;
    readfile.open(argv[v], ios::in);
    string reader;
    int divider1=0;
    int divider2=0;
    int finaldivide;

    EntryInterface temp;
    int ID;
    string word;
    string questionID;
    int questionFreq;
    vector<pair<string, int>> question;

    readfile >> reader; //should read "HashTable or AVL
    readfile >> reader; //should read questionsTotal
    questionsTotal = stoi(reader);
    readfile >> reader; //should read number of elements
    int wordNum = stoi(reader);

    for(int i=0; i<50; i++)
    {
        readfile >> reader;
        divider2 = reader.find('|');
        int frequency = stoi(reader.substr(divider1, divider2));
        string name = reader.substr((divider2+1), reader.size());

        pair<int, string> pairUp(frequency, name);
        mostFreq[i] = pairUp;
    }

    readfile >> reader; //should read "///////////"

    //from here on contains the rest of the stored words
    while(!readfile.eof())
    {
        readfile >> reader;

        //the first section is the location on the HashTable
        //divider1 should = 0
        divider2 = reader.find('|');
        ID = stoi(reader.substr(divider1, divider2));
        divider1 = (divider2+1);
        //this replaces the deliminator with another that is not used so the next "find('|')" will skip the '~'
        reader.replace(divider2, divider1, "~");


        //the second section is the actual word itself
        divider2 = reader.find('|');
        word = reader.substr(divider1, divider2);
        divider1 = (divider2+1);
        reader.replace(divider2, divider1, "~");

        //this finds where the last place the divide is
        finaldivide = reader.find_last_of('|');

        //adds to the vector until it hits the end
        while(divider2>=finaldivide)
        {
            divider2 = reader.find('-');
            questionID = reader.substr(divider1, divider2);
            divider1 = (divider2+1);
            reader.replace(divider2, divider1, "~");

            divider2 = reader.find('|');
            questionFreq = stoi( reader.substr(divider1, divider2) );
            divider1 = (divider2+1);
            reader.replace(divider2, divider1, "~");

            pair<string, int> instance(questionID, questionFreq);
            question.push_back(instance);
        }

        //completes the final instance
        divider2 = reader.find('-');
        questionID = reader.substr(divider1, divider2);
        divider1 = (divider2+1);
        reader.replace(divider2, divider1, "~");

        questionFreq = stoi( reader.substr(divider1) );
        divider1 = (divider2+1);

        pair<string, int> instance(questionID, questionFreq);
        question.push_back(instance);

        EntryInterface newEntry(word, question);
        hashIndex.addFromInput(ID, newEntry);

        divider1=0;
        divider2=0;
    }

    readfile.close();
}

void DocumentParser::getStatistics(bool indexMethod, AVLIndex<EntryInterface>& AVLindex, HashIndex<string, EntryInterface>& hashIndex)
{
    cout << "Total Number of Questions: " << questionsTotal <<endl;

    if(indexMethod==true)
    {
        cout << "Total Number of Words: " << AVLindex.size() <<endl;
        //mostFrequentAVL(AVLindex);
    }

    else
    {
        cout << "Total Number of Words: " << hashIndex.getSize() <<endl;
        //mostFrequentHash(hashIndex);
    }
    cout << endl;
    /*cout << "Top 50 Most Frequent Words: " <<endl;

    for(int i=0; i<50; i++)
    {
        cout << i << ": " << mostFreq[i].second <<endl;
    }*/
}
