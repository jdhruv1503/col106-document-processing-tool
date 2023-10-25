#include "search.h"

SearchEngine::SearchEngine() {

    cap = 1000;
    corpus.resize(cap);
    size = 0;

}

SearchEngine::~SearchEngine(){
    // no pointers, yayyyyyyyyyy!
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence) {

    // we only deal with lowercase in THIS town
    for (int i=0; i<sentence.size(); i++) {
        sentence[i] = tolower(sentence[i]);
    }
    
    size++;
    // dynamic allocation for corpus: faster than push back
    if (size == cap) {
        cap *= 1.5;
        corpus.resize(cap);
    }

    corpus.emplace_back(Sentence{book_code, page, paragraph, sentence_no, sentence});

}

Node* SearchEngine::search(string pattern, int& n_matches) {

    // Convert pattern to lowercase too

    for (int i=0; i<pattern.size(); i++) {
        pattern[i] = tolower(pattern[i]);
    }

    n_matches = 0;
    int patternLength = pattern.size();
    Node *rootnode = new Node();
    Node *currnode = rootnode;

    // Boyer-Moore algo

    std::vector<int> goodShiftArray(patternLength+1, 0);
    std::vector<int> badPositionArray(patternLength+1);

    // Initialize bad character array

    int i = patternLength;
    int j = patternLength+1;

    badPositionArray[patternLength] = patternLength+1;

    // Create bad character heuristic

    while(i > 0) {
        while(j <= patternLength && pattern[i-1] != pattern[j-1]) {

            if (goodShiftArray[j] == 0) {
                goodShiftArray[j] = j - i;
            }

            j = badPositionArray[j];
        }

        i--;
        j--;

        badPositionArray[i] = j;
        
    }

    // Create good suffix heuristic

    i = 0;
    j = badPositionArray[0];

    for(i=0; i<=patternLength; i++) {

        if(goodShiftArray[i] == 0) {
            goodShiftArray[i] = j;
        }

        if (i == j) {
            j = badPositionArray[j];
        }

    }

    // now search in the ENTIRE corpus

    for(int i=0; i<corpus.size(); i++) {

        int N = corpus[i].sentence.size();
        int s=0, j;

        while(s <= N-patternLength)
        {
            j = patternLength-1;

            while(j >= 0 && pattern[j] == corpus[i].sentence[s+j]) {
                j--;
            }

            if (j<0) {

                // build linked list and also increment n matches

                currnode->book_code = corpus[i].book_code;
                currnode->page = corpus[i].page;
                currnode->paragraph = corpus[i].paragraph;
                currnode->sentence_no = corpus[i].sentence_no;
                currnode->offset = s;

                currnode->right = new Node();
                currnode->right->left = currnode;
                currnode = currnode->right;

                n_matches++;

                s += goodShiftArray[0];

            }
            else
                s += goodShiftArray[j+1];
        }
    }

    if (n_matches == 0) {

        // this prevents memory leaks when no output is returned , HELL to debug :(

        delete rootnode;
        return NULL;
    }

    else {

        // delete the last, placeholder node and then return linked-list

        currnode->left->right = NULL;
        delete currnode;
        return rootnode;

    }
}