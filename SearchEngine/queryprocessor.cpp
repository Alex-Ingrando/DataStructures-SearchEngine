#include "queryprocessor.hpp"
using namespace std;
using namespace std::chrono;


QueryProcessor::QueryProcessor(){

}

void QueryProcessor::menu(){
    cout << "Welcome to the StackOverflow search engine." << endl << endl;
    string answer = "";
    while(answer != "0"){
        cout << "Main Menu" << endl;
        cout << "  Press 1 for Maintenance menu." << endl;
        cout << "  Press 2 for interactive menu." << endl;
        cout << "  Press 0 to quit the StackOverflow search engine" << endl << endl;
        getline(cin, answer);
        cout << endl;
        if(answer == "0"){
            cout << "Thank you for using the StackOverflow search engine" << endl << endl;
            index.writeIndex();
        } else if (answer == "1"){
            maitenceMode();
        } else if (answer == "2"){
            interactiveMode();
        }
    }
}

void QueryProcessor::interactiveMode(){
    string answer="";
    while(answer != "0"){
        cout << "Interactive Menu" << endl;
        cout << index.checkPersisted() + " index stored in file." << endl;
        if(!index.indexExists()){
            cout << "  Press 1 to load the index in an AVL tree." << endl;
            cout << "  Press 2 to load the index in an hash table." << endl;

        } else if(index.indexAVLTree()){
            cout << "  Index currently loaded in AVL Tree." << endl;
            cout << "  Press 2 to view index statistics" << endl;
        } else if(index.indexHashTable()){
            cout << "  Index currently loaded in HashTable." << endl;
            cout << "  Press 2 to view index statistics" << endl;
        }
        cout << "  Press 0 to return to the main menu." << endl << endl;
        if(index.indexExists())
            cout << "Enter your query to search the engine:";
        getline(cin, answer);
        cout << endl;
        if (answer == "1"){
            index.loadTree();
        } else if (answer == "2"){
            if(index.indexHashTable() || index.indexAVLTree())
                index.getStatistics();
            else
                index.loadTable();
        } else if (answer != "0"){
            search(answer);
        }

    }
}

void QueryProcessor::maitenceMode(){
    string answer="";
    while(answer != "0"){
        cout << "Maintenance Menu" << endl;
        //cout << "  Press 1 to add documents to the index" << endl;
        cout << "  Press 2 to delete the index." << endl;
        cout << "  Press 0 to return to the main menu." << endl << endl;
        getline(cin, answer);
        if(answer == "2"){
            index.deleteIndex();
        }
        cout << endl;
    }
}

void QueryProcessor::search(string answer){
    istringstream iss(answer, istringstream::in);
    string word;
    bool keyWord;
    vector<pair<string, int>> documents;
    cout << "Searching " << answer << "..." << endl;
    auto start = high_resolution_clock::now();
    while( iss >> word )
    {
        if(word == "AND") {
            //get first word to fill vector docs to compare
            iss >> word;
            Porter2Stemmer::stem(word);
            EntryInterface results = index.search(word);
            for(int i = 0; i < results.getFreq().size(); i++){
                bool inserted = false;
                if(documents.size() == 0){
                  documents.push_back(results.getFreq().at(i));
                } else {
                    for(int j = 0; j < 15 && j < documents.size(); j++){
                        if((static_cast<double>(results.getFreq().at(i).second)/static_cast<double>(results.getAmount())) > static_cast<double>(documents.at(j).second / results.getAmount())){
                            documents.insert(documents.begin() + j, results.getFreq().at(i));
                            documents.push_back(results.getFreq().at(i));
                            inserted = true;
                            break;
                        }
                    }
                    if(inserted == false){
                        documents.push_back(results.getFreq().at(i));
                    }
                }

            }
            while(iss >> word && word != "NOT"){
                Porter2Stemmer::stem(word);
                EntryInterface results = index.search(word);
                for(int i = 0; i < documents.size(); i++){
                    bool andDoc = false;
                    for(int j = 0; j < results.getFreq().size(); j++){
                        if(results.getFreq().at(j).first == documents.at(i).first){
                            andDoc = true;
                        }
                    }
                    if(andDoc == false){
                        documents.erase(documents.begin() + i);
                        i--;
                    }
                }
            }
            keyWord = true;
        }
        if (word == "OR"){
            //get first word to fill vector docs to compare
            iss >> word;
            Porter2Stemmer::stem(word);
            EntryInterface results = index.search(word);
            for(int i = 0; i < results.getFreq().size(); i++){
                bool inserted = false;
                if(documents.size() == 0){
                  documents.push_back(results.getFreq().at(i));
                } else {
                    for(int j = 0; j < 15 && j < documents.size(); j++){
                        if((static_cast<double>(results.getFreq().at(i).second)/static_cast<double>(results.getAmount())) > static_cast<double>(documents.at(j).second) / static_cast<double>(results.getAmount())){
                            documents.insert(documents.begin() + j, results.getFreq().at(i));
                            inserted = true;
                            break;
                        }
                    }
                    if(inserted == false){
                        documents.push_back(results.getFreq().at(i));
                    }
                }

            }
            while(iss >> word && word != "NOT"){
                Porter2Stemmer::stem(word);
                EntryInterface results = index.search(word);
                for(int i = 0; i < results.getFreq().size(); i++){
                    bool andDoc = false;
                    for(int j = 0; j < documents.size(); j++){
                        if(results.getFreq().at(i).first == documents.at(j).first){
                            andDoc = true;
                        }
                    }
                    if(andDoc == false){
                        documents.push_back(results.getFreq().at(i));
                    }
                }
            }
            keyWord = true;
        }

        if(word == "NOT"){
            iss >> word;
            Porter2Stemmer::stem(word);
            EntryInterface results = index.search(word);
            for(int i = 0; i < documents.size(); i++){
                bool andDoc = false;
                for(int j = 0; j < results.getFreq().size(); j++){
                    if(results.getFreq().at(j).first == documents.at(i).first){
                        andDoc = true;
                    }
                }
                if(andDoc == true){
                    documents.erase(documents.begin() + i);
                    i--;
                }
            }
            keyWord = true;
        }
        if(keyWord == false) {
            Porter2Stemmer::stem(word);
            EntryInterface results = index.search(word);
            for(int i = 0; i < results.getFreq().size(); i++){
                bool inserted = false;
                if(documents.size() == 0){
                  documents.push_back(results.getFreq().at(i));
                } else {
                    for(int j = 0; j < 15 && j < documents.size(); j++){
                        if((static_cast<double>(results.getFreq().at(i).second)/static_cast<double>(results.getAmount())) > (static_cast<double>(documents.at(j).second) / static_cast<double>(results.getAmount()))){
                            documents.insert(documents.begin() + j, results.getFreq().at(i));
                            inserted = true;
                            break;
                        }
                    }
                    if(inserted == false){
                        documents.push_back(results.getFreq().at(i));
                    }
                }

            }
        } else {
            keyWord = false;
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Found " << documents.size() << " results in " << duration.count() / 1000000.00 << " seconds." << endl;
    printResults(documents);
}

void QueryProcessor::printResults(vector<pair<string, int>>& documents){
    string answer = "";
    while(answer != "0"){
        for(int i = 0; i < documents.size() && i < 15; i++){
            string question = index.getDocument(documents.at(i).first).getName();
            cout << i+1 << ". " << question << endl;

        }
        cout << endl ;
        cout << "Press the corresponding number to open the document." << endl;
        cout << "Press 0 to return to the main menu." << endl << endl;
        getline(cin, answer);
        cout << endl;
        if(stoi(answer) <= 15 && stoi(answer) <= documents.size() && stoi(answer)> 0){
            string question = index.getDocument(documents.at(stoi(answer) - 1).first).getName();
            string body = index.getDocument(documents.at(stoi(answer) - 1).first).getFreq()[0].first;
            body.erase(std::remove(body.begin(), body.end(), '<'), body.end());
            body.erase(std::remove(body.begin(), body.end(), '>'), body.end());
            body.erase(std::remove(body.begin(), body.end(), '!'), body.end());
            body.erase(std::remove(body.begin(), body.end(), '?'), body.end());
            body.erase(std::remove(body.begin(), body.end(), '|'), body.end());

            cout << question << endl << endl;
            cout << body << endl << endl;
            cout << "Press 0 to return to interactive menu. Press anyother button to return to search results" << endl;
            getline(cin, answer);
        }
    }
}
