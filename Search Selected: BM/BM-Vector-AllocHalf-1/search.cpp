#include "search.h"

SearchEngine::SearchEngine(){
    cap = 1;
    corpus.resize(cap);
    size = 0;
}

SearchEngine::~SearchEngine(){
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){

    for (auto& ch : sentence) {
        ch = tolower(ch);
    }
    
    size++;

    if (size == cap) {
        cap *= 1.5;
        corpus.resize(cap);
    }

    corpus.emplace_back(Sentence{book_code, page, paragraph, sentence_no, sentence});

}

void preprocess_strong_suffix(int *shift, int *bpos, char *pat, int m)
{
    int i=m, j=m+1;
    bpos[i]=j;

    while(i>0)
    {
        while(j<=m && pat[i-1] != pat[j-1])
        {
            if (shift[j]==0)
                shift[j] = j-i;

            j = bpos[j];
        }
        i--;j--;
        bpos[i] = j;
    }
}

void preprocess_case2(int *shift, int *bpos, char *pat, int m)
{
    int i, j;
    j = bpos[0];
    for(i=0; i<=m; i++)
    {
        if(shift[i]==0)
            shift[i] = j;

        if (i==j)
            j = bpos[j];
    }
}


Node* SearchEngine::search(string pattern, int& n_matches) {
    // Convert pattern to lowercase
    for (auto& ch : pattern) {
        ch = tolower(ch);
    }

    n_matches = 0;
    int M = pattern.size();
    Node *rootnode = new Node();
    Node *currnode = rootnode;

    int bpos[M+1], shift[M+1];
    for(int i=0;i<M+1;i++) shift[i]=0;

    preprocess_strong_suffix(shift, bpos, &pattern[0], M);
    preprocess_case2(shift, bpos, &pattern[0], M);

    for(const auto& sentence : corpus) {
        int N = sentence.sentence.size();
        int s=0, j;

        while(s <= N-M)
        {
            j = M-1;

            while(j >= 0 && pattern[j] == sentence.sentence[s+j])
                j--;

            if (j<0)
            {
                currnode->book_code = sentence.book_code;
                currnode->page = sentence.page;
                currnode->paragraph = sentence.paragraph;
                currnode->sentence_no = sentence.sentence_no;
                currnode->offset = s;
                currnode->right = new Node();
                currnode->right->left = currnode;
                currnode = currnode->right;
                n_matches++;
                s += shift[0];
            }
            else
                s += shift[j+1];
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
