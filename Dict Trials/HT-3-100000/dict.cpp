// Do NOT add any other includes
#include "dict.h"

HashTable::HashTable(int size) {
    table_size = size;
    num_entries = 0;
    table.resize(table_size);
}

int HashTable::hashFunction(string str) {
    int hash = 0;
    for (char c : str) {
        hash ^= static_cast<int>(c);
        hash %= table_size;
    }
    return hash;
}

void HashTable::handleCollision(int index, string word) {
    for (Entry &entry : table[index]) {
        if (entry.word == word) {
            entry.count++;
            return;
        }
    }
    table[index].push_back({word, 1});
    num_entries++;
    if (num_entries > table_size / 2) {
        resizeTable();
    }
}

void HashTable::insertWord(string word) {
    int index = hashFunction(word);
    handleCollision(index, word);
}

int HashTable::getWordCount(string word) {
    int index = hashFunction(word);
    for (const Entry &entry : table[index]) {
        if (entry.word == word) {
            return entry.count;
        }
    }
    return 0;
}

void HashTable::dumpDictionary(string filename) {
    ofstream file;
    file.open(filename);
    for (const vector<Entry> &bucket : table) {
        for (const Entry &entry : bucket) {
            file << entry.word << ", " << entry.count << "\n";
        }
    }
    file.close();
}

void HashTable::resizeTable() {
    vector<vector<Entry>> old_table = table;
    table_size *= 2;
    num_entries = 0;
    table.clear();
    table.resize(table_size);
    for (const vector<Entry> &bucket : old_table) {
        for (const Entry &entry : bucket) {
            for (int i = 0; i < entry.count; i++) {
                insertWord(entry.word);
            }
        }
    }
}

Dict::Dict(){
    hashTable = new HashTable(100000);
}

Dict::~Dict(){
    delete hashTable;
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence) {
    string cw = "";

    for (int i=0; i<sentence.size(); i++) {
        if (isalnum(sentence[i])) {
            cw.push_back(tolower(sentence[i]));
        }
        else {
            if (cw != "") {
                hashTable->insertWord(cw);
            }
            cw = "";
        }
    }

    if (cw != "") {
        hashTable->insertWord(cw);
    }
}


int Dict::get_word_count(string word){
    return hashTable->getWordCount(word);
}

void Dict::dump_dictionary(string filename){
    return hashTable->dumpDictionary(filename);

}

// ------------------ DEBUG ZONE, REMOVE FOR FINAL SUBMISSION -----------------------------
#include <fstream>
#include <string>

std::string readFileIntoString(const std::string& path) {
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '" << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::string((std::istreambuf_iterator<char>(input_file)),
                       std::istreambuf_iterator<char>());
}

int main() {
    
    Dict* mydick = new Dict();

    string instirng = readFileIntoString("corpus.txt");

    mydick->insert_sentence(0, 0, 0, 0, instirng);
    mydick->dump_dictionary("output.txt");

    delete mydick;
    
    return 0;
}