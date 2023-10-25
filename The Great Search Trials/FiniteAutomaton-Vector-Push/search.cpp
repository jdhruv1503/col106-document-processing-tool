#include "search.h"

const int NO_OF_CHARS = 256;

SearchEngine::SearchEngine(){
    corpus = {};
}

SearchEngine::~SearchEngine(){
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){

    for (auto& ch : sentence) {
        ch = tolower(ch);
    }
    
    corpus.push_back({book_code, page, paragraph, sentence_no, sentence});

}

#define NO_OF_CHARS 256 
 
int getNextState(string pat, int M, int state, int x) 
{ 
    // If the character c is same as next character 
    // in pattern,then simply increment state 
    if (state < M && x == pat[state]) 
        return state+1; 
 
    // ns stores the result which is next state 
    int ns, i; 
 
    // ns finally contains the longest prefix 
    // which is also suffix in "pat[0..state-1]c" 
 
    // Start from the largest possible value 
    // and stop when you find a prefix which 
    // is also suffix 
    for (ns = state; ns > 0; ns--) 
    { 
        if (pat[ns-1] == x) 
        { 
            for (i = 0; i < ns-1; i++) 
                if (pat[i] != pat[state-ns+1+i]) 
                    break; 
            if (i == ns-1) 
                return ns; 
        } 
    } 
 
    return 0; 
} 
 
/* This function builds the TF table which represents4 
    Finite Automata for a given pattern */
void computeTF(string pat, int M, int TF[][NO_OF_CHARS]) 
{ 
    int state, x; 
    for (state = 0; state <= M; ++state) 
        for (x = 0; x < NO_OF_CHARS; ++x) 
            TF[state][x] = getNextState(pat, M, state, x); 
} 
 
/* Prints all occurrences of pat in txt */
vector<int> searchA(string pat, string txt) 
{ 
    vector<int> occ = {};
    int M = pat.size(); 
    int N = txt.size(); 
 
    int TF[M+1][NO_OF_CHARS]; 
 
    computeTF(pat, M, TF); 
 
    // Process txt over FA. 
    int i, state=0; 
    for (i = 0; i < N; i++) 
    { 
        state = TF[state][txt[i]]; 
        if (state == M) 
            occ.push_back(i-M+1); 
    } 
    return occ;
} 

Node* SearchEngine::search(string pattern, int& n_matches) {

    for (auto& ch : pattern) {
        ch = tolower(ch);
    }

        n_matches = 0;
        int M = pattern.size();
        Node *rootnode = new Node();
        Node *currnode = rootnode;

        for(const auto& sentence : corpus) {
            vector<int> N = searchA(pattern, sentence.sentence);

            for(const auto& i : N) {// if pattern[0...M-1] = sentence[i, i+1, ...i+M-1]
                    currnode->book_code = sentence.book_code;
                    currnode->page = sentence.page;
                    currnode->paragraph = sentence.paragraph;
                    currnode->sentence_no = sentence.sentence_no;
                    currnode->offset = i;
                    currnode->right = new Node();
                    currnode->right->left = currnode;
                    currnode = currnode->right;
                    n_matches++;
                
            }
        }

        if (n_matches == 0) {
            delete rootnode;
        return NULL;
        }
        else {
            currnode->left->right = NULL;
            delete currnode;
            return rootnode;
        }
    }



void traverse(Node* node) {
    int k = 1;
    while(node != NULL) {
        cout << k << ". " << node->book_code << "|" << node->page << "|" << node->paragraph << "|" << node->sentence_no << "|" << node->offset << endl;
        Node* nextNode = node->right;
        node = nextNode;
        k++;
    }
}

// ------------ DEBUG ZONE
#include <fstream>
#include <sstream>
using namespace std;

int main() {
    string filepath = "corpus.txt";
    SearchEngine* se = new SearchEngine();
    ifstream file(filepath);
    string line;
    while (std::getline(file, line)) {
        int pos = line.find(") ");
        string sent = line.substr(pos+2, line.size() - pos - 1);
        string params = line.substr(2, pos-2);

        int a = stoi(params.substr(0, params.find('\'')));
        params = params.substr(params.find(',')+1, params.size()-params.find(','));
        int b = stoi(params.substr(0, params.find(',')));
        params = params.substr(params.find(',')+1, params.size()-params.find(','));
        int c = stoi(params.substr(0, params.find(',')));
        params = params.substr(params.find(',')+1, params.size()-params.find(','));
        int d = stoi(params.substr(0, params.find(',')));
        params = params.substr(params.find(',')+1, params.size()-params.find(','));
        int e = stoi(params);

        se->insert_sentence(a,b,c,d,sent);

    }

    int n = 0;

    Node* res = se->search("lump", n);
    traverse(res);

    while (res != NULL) {
        Node* nex = res->right;
        delete res;
        res = nex;
    }

    cout << n << endl;

    res = se->search("talk with him", n);
    traverse(res);

    while (res != NULL) {
        Node* nex = res->right;
        delete res;
        res = nex;
    }

    cout << n << endl;

    res = se->search("inexp", n);
    traverse(res);

    while (res != NULL) {
        Node* nex = res->right;
        delete res;
        res = nex;
    }

    cout << n << endl;

    delete se;
    
}
