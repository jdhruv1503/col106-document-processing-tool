#include "search.h"

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

Node* SearchEngine::search(string pattern, int& n_matches) {
    for (auto& ch : pattern) {
        ch = tolower(ch);
    }
    Node *rootnode = new Node();
    Node *currnode = rootnode;
    int d = 256;  // number of characters in the input alphabet
    int q = 101;  // a prime number
    int m = pattern.size();
    int i, j;
    int p = 0;  // hash value for pattern
    int t = 0;  // hash value for text
    int h = 1;

    // The value of h would be "pow(d, m-1)%q"
    for (i = 0; i < m - 1; i++)
        h = (h * d) % q;

    // Slide the pattern over text one by one
    for (i = 0; i < corpus.size(); i++) {
        int N = corpus[i].sentence.size();
        for (j = 0; j <= N - m; j++) {
            // Calculate the hash value of pattern and current window of text
            p = 0;
            t = 0;
            for (int k = 0; k < m; k++) {
                p = (d * p + pattern[k]) % q;
                t = (d * t + corpus[i].sentence[j + k]) % q;
            }

            // Check the hash values of current window of text and pattern
            // If the hash values match then only check for characters one by one
            if (p == t) {
                int k;
                for (k = 0; k < m; k++) {
                    if (corpus[i].sentence[j + k] != pattern[k])
                        break;
                }

                // if p == t and pattern[0...m-1] = text[i, i+1, ...i+m-1]
                if (k == m) {
                    currnode->book_code = corpus[i].book_code;
                    currnode->page = corpus[i].page;
                    currnode->paragraph = corpus[i].paragraph;
                    currnode->sentence_no = corpus[i].sentence_no;
                    currnode->offset = i;
                    currnode->right = new Node();
                    currnode->right->left = currnode;
                    currnode = currnode->right;
                    n_matches++;
                }
            }
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
