// Do NOT add any other includes
#include "dict.h"

HashTable::HashTable(int size) {
    table_size = size;
    num_entries = 0;
    table.resize(table_size);
}

int HashTable::hashFunction(string word) {
    // take ascii val after '0' and store as base-(122-48+1)=BASE75 number

    int sum = 0;

    for (int i=0; i<word.size(); i++) {
        
        sum = sum*73 + int(word[i]) - int('0');

        sum = sum % table_size;

    }

    return sum;
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
    hashTable = new HashTable(109121);
}

Dict::~Dict(){
    delete hashTable;
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence) {
    // split the sentence into words
    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= sentence.size(); i++) {
        // If we reached the end of the word or the end of the input.
        if (sentence[i] == ' ' || i == sentence.size()) {
            endIndex = i;
            string word;
            word.append(sentence, startIndex, endIndex - startIndex);

            for (char &c : word) {
                c = tolower(c);
            }

            hashTable->insertWord(word);
            startIndex = endIndex + 1;
        }
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