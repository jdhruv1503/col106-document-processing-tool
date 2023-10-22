// Do NOT add any other includes
#include "dict.h"

Dict::Dict(){
    root = new TrieNode();
}

void freeTrie(TrieNode* root) {
    for (int i = 0; i < 36; i++)
        if (root->children[i])
            freeTrie(root->children[i]);
    delete root;
}

Dict::~Dict() {
    freeTrie(root);
}


int charToIndex(char c) {
    if(c >= 'a' && c <= 'z') return c - 'a';
    if(c >= '0' && c <= '9') return c - '0' + 26;
}

TrieNode *getNode() {
    TrieNode *node = new TrieNode;

    node->isEndOfWord = false;
    node->wordCount = 0;

    for (int i = 0; i < 36; i++)
        node->children[i] = nullptr;

    return node;
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence) {
    string word;
    TrieNode* pCrawl = root;
    for (char c : sentence) {
        if (isalnum(c)) {
            c = tolower(c);
            int index = charToIndex(c);
            if (!pCrawl->children[index])
                pCrawl->children[index] = getNode();
            pCrawl = pCrawl->children[index];
            word.push_back(c);
        } else {
            if (!word.empty()) {
                pCrawl->isEndOfWord = true;
                pCrawl->wordCount++;
                word.clear();
                pCrawl = root;
            }
        }
    }
    if (!word.empty()) {
        pCrawl->isEndOfWord = true;
        pCrawl->wordCount++;
    }
}




int Dict::get_word_count(string word) {
    TrieNode *pCrawl = root;
    for (char c : word) {
        c = tolower(c);
        int index = charToIndex(c);
        if (!pCrawl->children[index])
            return 0;
        pCrawl = pCrawl->children[index];
    }
    if (pCrawl != nullptr && pCrawl->isEndOfWord)
        return pCrawl->wordCount;
    return 0;
}



void printTrie(TrieNode* root, string str, ofstream &file) {
    if (root->isEndOfWord)
        file << str << ", " << root->wordCount << "\n";
    for (char c = 'a'; c <= 'z'; c++)
        if (root->children[c - 'a'])
            printTrie(root->children[c - 'a'], str + c, file);
    for (char c = '0'; c <= '9'; c++)
        if (root->children[c - '0' + 26])
            printTrie(root->children[c - '0' + 26], str + c, file);
}

void Dict::dump_dictionary(string filename) {
    ofstream file;
    file.open(filename);
    printTrie(root, "", file);
    file.close();
}