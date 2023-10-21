#include "Node.h"

Node::Node() {

    Node* left = NULL;
    Node* right = NULL;

}

Node::Node(int b_code, int pg, int para, int s_no, int off){

    Node* left = NULL;
    Node* right = NULL;

    book_code = b_code;
    page = pg;
    paragraph = para;
    sentence_no = s_no;
    offset = off;
    
}