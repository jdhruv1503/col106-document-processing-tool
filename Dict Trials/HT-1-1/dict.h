// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

struct Entry {
    string word;
    int count;
};

class HashTable {
    vector<vector<Entry>> table;
    int table_size;
    int num_entries;

    // other helper functions

public:
    HashTable(int size);
    void resizeTable();
    int hashFunction(string word);
    void insertWord(string word);
    int getWordCount(string word);
    void dumpDictionary(string filename);
    void handleCollision(int index, string word);
};

class Dict {
private:
    // You can add attributes/helper functions here
    HashTable* hashTable;

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};