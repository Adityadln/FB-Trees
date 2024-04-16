#include <bits/stdc++.h>
using namespace std;
#define MAX_NODES 5
class Node{
    bool IS_LEAF;
    vector<int>keys;
    int size;
    vector<Node*>ptr;
    friend class BPTree;
    public:
    vector<CachePair> branchRanges;
    Node(){
        keys.resize(MAX_NODES);
        ptr.resize(MAX_NODES + 1);
    }
};
class CachePair {
public:
    int bmin; 
    int bmax; 
    Node* next; 
    CachePair(int min, int max, Node* nextNode = nullptr) : bmin(min), bmax(max), next(nextNode) {}
};


class BPTree{
    Node* root;
    void insertInternal(int,Node*,Node*);
    Node* findParent(Node*,Node*);
    public:
    BPTree(){
        root = NULL;
    }
    void search(int);
    void insert(int);
    void display(Node*);
    Node* getRoot();
};
Node* BPTree::getRoot(){
    //* gets the root of the B+ Tree
    return root;
}
void BPTree::search(int key){
    
    if(root == NULL){
        cout<<"The tree is empty..No element found!"<<endl;
    }
    else{
        Node* cursor = root;
        while(cursor->IS_LEAF == false){
            for(int i = 0;i < cursor->size;i++){
                if (key >= cursor->branchRanges[i].bmin && key <= cursor->branchRanges[i].bmax) {
                    cursor = cursor->ptr[i];
                    break;
                }
                if (i == cursor->size && key > cursor->branchRanges[i - 1].bmax) {
                cout << "key " << key << " does not exist (early termination)" << endl;
                return;
                }
            }
        }
        //* reached the correct child node
        for(int i = 0;i < cursor->size;i++){
            if(cursor->keys[i] == key){
                cout<<"key "<<key<<" has been found"<<endl;
                return;
            }
        }
        cout<<"key "<<key<<" has not been found"<<endl;
    }
}
void BPTree::insert(int x){
    if(root == NULL){
        //* if root itself is not present
        root = new Node;
        root->keys[0] = x;
        root->size = 1;
        root->IS_LEAF = true;
        root->branchRanges.push_back(CachePair(x, x));
    }
    else{
        Node* cursor = root;
        Node* parent;
        //* starting from root
        while(cursor->IS_LEAF == false){
                parent = cursor;
                for (int i = 0; i < cursor->size; i++) {
                    if (x >= cursor->branchRanges[i].bmin && x <= cursor->branchRanges[i].bmax) {
                    cursor = cursor->ptr[i];
                    break;
                    }
                }
        }
        //* if size is not maxed
        if(cursor->size < MAX_NODES){
            int i,j;
            //* go until elements lesser than new-key
            for(i = 0;i < cursor->size;i++){
                if(x < cursor->keys[i])break;
            }
            int prev = cursor->keys[i];
            for(int j = i + 1;j < cursor->size;j++){
                int temp = cursor->keys[j];
                cursor->keys[j] = prev;
                prev = temp;
            }
            cursor->keys[i] = x;
            cursor->size++;
            //* whatever points to the next leaf node is shifted from cursor->size - 1 to cursor->size as an element is added
            cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
            cursor->ptr[cursor->size - 1] = NULL;  

            //*cache range updation
            if (i == 0) {
                cursor->branchRanges[0].bmin = x; 
            } else if (i == cursor->size - 1) {
                cursor->branchRanges[i - 1].bmax = x; 
            } else {
                //*inserts the cache range at position (1st argument) and inserts (x,x) 
                cursor->branchRanges.insert(cursor->branchRanges.begin() + i, CachePair(x, x));
            }

        }
        else{
            Node* newleaf = new Node;
            //* split the node
            vector<int>tempkeys(MAX_NODES + 1);
            for(int i = 0;i < MAX_NODES;i++){
                tempkeys[i] = cursor->keys[i];
            }
            int j;
            for(j = 0;j < MAX_NODES;j++){
                if(cursor->keys[j] > x)break;
            }
            for(int k = MAX_NODES + 1;k > j;k++){
                tempkeys[k] = tempkeys[k - 1];
            }
            tempkeys[j] = x;
            newleaf->IS_LEAF = true;
            cursor->size = (MAX_NODES + 1)/2;
            newleaf->size = MAX_NODES - (MAX_NODES + 1)/2;
            for(int i = 0;i < cursor->size;i++){
                cursor->keys[i] = tempkeys[i];
            }
            for(int i = 0,j = cursor->size;i < newleaf->size;i++,j++){
                newleaf->keys[i] = tempkeys[j];
            }
            //* spliited into cursor on the left,and the newleaf on the right
            if(cursor == root){
                Node* newRoot = new Node;
                newRoot->keys[0] = newleaf->keys[0];
                newRoot->ptr[0] = cursor;
                newRoot->ptr[1] = newleaf;
                newRoot->IS_LEAF = false;
                newRoot->size = 1;
                root = newRoot;
                newRoot->branchRanges.push_back(CachePair(tempkeys[cursor->size], tempkeys[cursor->size]));
            }
            else{
               insertInternal(newleaf->keys[0],parent,newleaf); 
            }  
        }  
    }
}
void BPTree::insertInternal(int x,Node* cursor,Node* child){
    if(cursor->size < MAX_NODES){
        int i;
        for(i = 0;i < cursor->size;i++){
            if(cursor->keys[i] > x)break;
        }
        for(int j = cursor->size;j > i;j--){
            cursor->keys[j] = cursor->keys[j - 1];
        }
        for(int k = cursor->size + 1;k > i + 1;k--){
            cursor->ptr[k] = cursor->ptr[k - 1];
        }
        cursor->keys[i] = x;
        cursor->size++;
        cursor->ptr[i + 1] = child;
    }
    else{
        Node* newInternal = new Node;
        vector<int>tempkeys(MAX_NODES + 1);
        vector<Node*>tempptr(MAX_NODES + 2);
        vector<CachePair> tempBranchRanges(MAX_NODES + 1);
        for(int i = 0;i < MAX_NODES;i++){
            tempkeys[i] = cursor->keys[i];
            tempBranchRanges[i] = cursor->branchRanges[i];
        }
        for(int j = 0;j < cursor->size + 1;j++){
            tempptr[j] = cursor->ptr[j];
        }
        int k;
        for(k = 0;k < MAX_NODES;k++){
            if(x > tempkeys[k])break;
        }
        for(int j = MAX_NODES + 1;j > k;j--){
            tempkeys[j] = tempkeys[j - 1];
        }
        tempkeys[k] = x;
        for(int j = MAX_NODES + 2;j > k + 1;j--){
            tempptr[j] = tempptr[j - 1];
        }
        tempptr[k] = child;
        //!!!!!!!!
        CachePair newCachePair(cursor->keys[0], cursor->keys[cursor->size - 1], newInternal);
        cursor->branchRanges.push_back(newCachePair);

        newInternal->IS_LEAF = false;
        cursor->size = (MAX_NODES + 1)/2;
        newInternal->size = MAX_NODES - (MAX_NODES + 1)/2;
        for (int i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {
        newInternal->keys[i] = tempkeys[j];
    }
        for (int i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
        newInternal->ptr[i] = tempptr[j];
        }
        if(cursor == root){
            Node* newroot = new Node;
            newroot->keys[0] = cursor->keys[0];
            newroot->ptr[0] = cursor;
            newroot->ptr[1] = newInternal;
            newroot->IS_LEAF = false;
            newroot->size = 1;
            root = newroot;
        }
        else{
            insertInternal(cursor->keys[0],findParent(root,cursor),newInternal);
        }
    }
}
Node* BPTree::findParent(Node*cursor,Node* child){
    //* null returned if `child` ptr is not actually child of any node
    if(cursor->IS_LEAF == true )return NULL;
    for(int i = 0;i < cursor->size + 1;i++){
        if(cursor->ptr[i] == child){
            //* if cursor is the parent of `child` return it.
            return cursor;
        }
        else{
            //* or search depth first and if not null returned ,return the parent node
            Node* parent = findParent(cursor->ptr[i],child);
            if(parent != NULL)return parent;
        }
    }
    return NULL;
}
void BPTree::display(Node* cursor){
    if(cursor != NULL){
        //* display all the keys in a node
        for(int i = 0;i < cursor->size;i++){
            cout<<cursor->keys[i]<<" ";
        }
        cout<<endl;
        //* go to its children one pointer at a time.
        if(cursor->IS_LEAF == false){
            for(int i = 0;i < cursor->size + 1;i++){
              display(cursor->ptr[i]);
            }
        }
    }
}
int main(){
    BPTree tree;
    tree.insert(15);
    tree.insert(25);
    tree.insert(35);
    tree.insert(45);
    tree.insert(55);
    tree.insert(40);
    tree.insert(30);
    tree.insert(20);
    tree.insert(5);
    tree.display(tree.getRoot());
    tree.search(15);
}



