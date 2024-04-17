#include "B++.cpp"
class cacheEntry;
class FBnode;
class FBTree{
    public:
    FBnode* root;
    void build_FB(Node* ,cacheEntry*,BPTree*);
    void build_FB_init(BPTree*);
    void displayFB();
    vector<int> search_range(pair<int,int>,FBnode*);
    bool search_point(int,FBnode*);

};
class FBnode{
    public:
    vector<cacheEntry>cache;
    int size;
    friend class FBTree;
    bool IS_LEAF;
    FBnode(){
        size = 0;
    }

};
class cacheEntry{
    public:
    int minval;
    int maxval;
    Node* bplusleaf;
    FBnode* fbchild;
    cacheEntry(){
        bplusleaf = NULL;
        fbchild = NULL;
    }
};
void FBTree::displayFB() {
    if (root == NULL) {
        cout << "Tree is empty" << endl;
        return;
    }
    queue<FBnode*> q;
    q.push(root);
    while (!q.empty()) {
      int size = q.size();
      for (int i = 0; i < size; i++) {
        FBnode* node = q.front();
        q.pop();
        cout<<"( ";
        for (int j = 0; j < node->size; j++) {
            cout << "< "<<node->cache[j].minval << "," << node->cache[j].maxval<< ">" <<" ";
            if (node->cache[j].fbchild != NULL) {
                q.push(node->cache[j].fbchild);
            }
        }
        cout<<" )";
        }
        cout << endl;
    }
}
vector<int> FBTree::search_range(pair<int,int>range, FBnode* cursor){
    Node* bplusnode;
    while(cursor->IS_LEAF == false){
        int i;
        for(i = 0;i < cursor->size;i++){
            if(range.first > cursor->cache[i].minval){
                cursor = cursor->cache[i].fbchild;
                break;
            }
            if(i == cursor->size - 1){
                if(range.first > cursor->cache[i].maxval)return vector<int>();
                cursor = cursor->cache[i + 1].fbchild;
            }
        }
        if(cursor->IS_LEAF == true){
            bplusnode = cursor->cache[i].bplusleaf;
        }
    }
    vector<int>ans;
    while(bplusnode != NULL){
        for(int i = 0;i < bplusnode->size;i++){
            if(range.second >= bplusnode->keys[i])ans.push_back(bplusnode->keys[i]);
            else return ans;
        }
        bplusnode = bplusnode->ptr[bplusnode->size];
    }
    return ans;
    
}

bool FBTree::search_point(int val, FBnode* cursor){
    Node* bplusnode;
    while(cursor->IS_LEAF == false){
        int i;
        for(i = 0;i < cursor->size;i++){
            if(val >= cursor->cache[i].minval && val <= cursor->cache[i].maxval){
                cursor = cursor->cache[i].fbchild;
                break;
            }
            else return false;
            if(i == cursor->size - 1){
                if(val > cursor->cache[i].maxval)return false;
                cursor = cursor->cache[i + 1].fbchild;
            }
        }
        if(cursor->IS_LEAF == true){
            bplusnode = cursor->cache[i].bplusleaf;
        }
    }
    for(int i = 0;i < bplusnode->size;i++){
        if(bplusnode->keys[i] == val)return true;
    }
    return false;    
}

int BPTree::minval(Node* cursor){
    if(cursor->IS_LEAF == true){
        return cursor->keys[0];
    }
    return minval(cursor->ptr[0]);
}
int BPTree::maxval(Node* cursor){
    if(cursor->IS_LEAF == true){
        return cursor->keys[cursor->size - 1];
    }
    return maxval(cursor->ptr[cursor->size]);
}
void FBTree::build_FB_init(BPTree* tree){
    Node* rootn = tree->root;
    cacheEntry *ce = new cacheEntry;
    FBnode *fbnode = new FBnode;
    root = fbnode;
    ce->minval = tree->minval(rootn);
    ce->maxval = tree->maxval(rootn);
    fbnode->cache.push_back(*ce);
    fbnode->size++;
    fbnode->IS_LEAF = false;
    cout<<" (< ";
    cout<<ce->minval<<" ";
    cout<<ce->maxval<<" ";
    cout<<" >)"<<endl;
    build_FB(rootn,ce,tree);

}
void FBTree::build_FB(Node* bptreenode,cacheEntry* ce,BPTree* tree){
    FBnode *fbnode = new FBnode;
    int bpsize = bptreenode->size;
    for(int i = 0;i <= bpsize;i++){
        cacheEntry *childce = new cacheEntry; 
        Node* child = bptreenode->ptr[i];
        childce->minval = tree->minval(child);
        childce->maxval = tree->maxval(child);
        fbnode->cache.push_back(*childce);
        fbnode->size++;
        if(child->IS_LEAF == true){
            fbnode->IS_LEAF = true;
            childce->bplusleaf = child;
        }
        else{
            fbnode->IS_LEAF = false;
            build_FB(child,childce,tree);

        }
    }
    
    ce->fbchild = fbnode;
    cout<<"( ";
    for(int j = 0;j < fbnode->size;j++){
        cout<<"< ";
        cout<<fbnode->cache[j].minval<<" ";
        cout<<fbnode->cache[j].maxval;
        cout<<" >";
    }
    cout<<" )";
}
int main(){
  BPTree node;
  node.insert(5);
  node.insert(15);
  node.insert(25);
  node.insert(35);
  node.insert(45);
  node.insert(55);
  node.insert(40);
  node.insert(30);
  node.insert(20);
  node.display();
  node.search(15);
  node.display();
  node.search(35);
  node.maxval(node.root);
  FBTree tree;
  tree.build_FB_init(&node);
  }


