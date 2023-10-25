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

vector<int> computeLPSArray(string s) {
vector<int> p(s.size());
int j = 0;
for (int i = 1; i < (int)s.size(); i++) {
    while (j > 0 && s[j] != s[i])
        j = p[j-1];

    if (s[j] == s[i])
        j++;
    p[i] = j;
}   
return p;
}


Node* SearchEngine::search(string pattern, int& n_matches) {
    for (auto& ch : pattern) {
        ch = tolower(ch);
    }

    n_matches = 0;
    int M = pattern.size();
    vector<int> lps = computeLPSArray(pattern);
    Node *rootnode = new Node();
    Node *currnode = rootnode;

    for(const auto& sentence : corpus) {
        int N = sentence.sentence.size();
        int i = 0;
        int j = 0;
        while (i < N) {
            if (pattern[j] == sentence.sentence[i]) {
                j++;
                i++;
            }
            if (j == M) {
                currnode->book_code = sentence.book_code;
                currnode->page = sentence.page;
                currnode->paragraph = sentence.paragraph;
                currnode->sentence_no = sentence.sentence_no;
                currnode->offset = i - j;
                currnode->right = new Node();
                currnode->right->left = currnode;
                currnode = currnode->right;
                n_matches++;
                j = lps[j - 1];
            } else if (i < N && pattern[j] != sentence.sentence[i]) {
                if (j != 0)
                    j = lps[j - 1];
                else
                    i = i + 1;
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
