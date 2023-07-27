#include "indexhandler.hpp"

IndexHandler::IndexHandler(){
    indexExist = false;
    indexTree = false;
    indexTable = false;
    parser.loadFileNames();
}

string IndexHandler::checkPersisted(){
    string persisted = parser.checkPersisted(0);
    if(persisted == "AVL Tree")
        persistedTree = true;
    else if (persisted == "Hash Table")
        persistedTable = true;
    return persisted;
}

void IndexHandler::loadTree(){
    indexExist = true;
    indexTree = true;
    parser.loadFileNames();
    treeIndex = AVLIndex<EntryInterface>();
    /*if(persistedTree){
        parser.inputAVL(treeIndex, 0);
        parser.runParser("Persisted", tableIndex, documentIndex, treeIndex);
    } else {*/
        parser.runParser("1", tableIndex, documentIndex, treeIndex);
   // }
}

void IndexHandler::loadTable(){
    indexExist = true;
    indexTable = true;
    /*if(persistedTable){
        parser.inputHash(tableIndex, 0);
        parser.runParser("Persisted", tableIndex, documentIndex, treeIndex);
    } else{*/
        parser.runParser("2", tableIndex, documentIndex, treeIndex);
    //}

}

bool IndexHandler::indexExists(){
    return indexExist;
}

bool IndexHandler::indexAVLTree(){
    return indexTree;
}

bool IndexHandler::indexHashTable(){
    return indexTable;
}

EntryInterface IndexHandler::getDocument(string id){
    EntryInterface test = documentIndex.at(stoi(id));
    return test;
}

EntryInterface IndexHandler::search(string word){
    if(indexTree == true)
        return searchTree(word);
    else if (indexTable == true)
        return searchTable(word);
}

EntryInterface IndexHandler::searchTree(string word){
    EntryInterface temp(word);
    bool stored = treeIndex.inTree(temp);
    if(stored == true)
        temp = treeIndex.search(temp);
    return temp;
}

EntryInterface IndexHandler::searchTable(string word){
    EntryInterface example = tableIndex.find(word);
    return example;
}


void IndexHandler::getStatistics(){
    parser.getStatistics(indexTree, treeIndex, tableIndex);
}

void IndexHandler::writeIndex(){
    if(indexTree){
        parser.outputAVL(treeIndex, 0);
    } else if(indexTable){
        parser.outputHash(tableIndex, 0);
    }
}

void IndexHandler::deleteIndex(){
    if(indexTree){
        treeIndex.deleteIndex();
    } else if (indexTable){
        tableIndex.deleteIndex();
    }
    indexExist = false;
    indexTree = false;
    indexTable = false;
    cout << "Index deleted." << endl;
}
