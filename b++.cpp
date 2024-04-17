#include <bits/stdc++.h>
using namespace std;
int ORDER = 3;
class Node {
   public:
  bool IS_LEAF;
  int size;
  vector<int>keys;
  vector<Node*>ptr;
  friend class BPTree;

  Node(){
    keys.resize(ORDER,-1);
    ptr.resize(ORDER + 1,NULL);
    size = 0;
    IS_LEAF = false;
  }
};
class BPTree {
  void insertInternal(int, Node *, Node *);
  Node *findParent(Node *, Node *);
   public:
  BPTree();
  Node *root;
  int minval(Node*);
  int maxval(Node*);
  void search(int);
  void insert(int);
  void display();
  Node *getRoot();
  friend class FBTree;
};
BPTree::BPTree() {
  root = NULL;
}

void BPTree::search(int x) {
  if (root == NULL) {
    cout << "Tree is empty\n";
  } else {
    Node *cursor = root;
    while (cursor->IS_LEAF == false) {
      for (int i = 0; i < cursor->size; i++) {
        if (x < cursor->keys[i]) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    for (int i = 0; i < cursor->size; i++) {
      if (cursor->keys[i] == x) {
        cout <<x<< " is Found\n";
        return;
      }
    }
    cout <<x<<" is Not found\n";
  }
}
void BPTree::insert(int x) {
  if (root == NULL) {
    root = new Node;
    root->keys[0] = x;
    root->IS_LEAF = true;
    root->size = 1;
  } else {
    Node *cursor = root;
    Node *parent;
    while (cursor->IS_LEAF == false) {
      parent = cursor;
      for (int i = 0; i < cursor->size; i++) {
        if (x < cursor->keys[i]) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    if (cursor->size < ORDER) {
      int i = 0;
      while (x > cursor->keys[i] && i < cursor->size)
        i++;
      for (int j = cursor->size; j > i; j--) {
        cursor->keys[j] = cursor->keys[j - 1];
      }
      cursor->keys[i] = x;
      cursor->size++;
      cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
      cursor->ptr[cursor->size - 1] = NULL;
    } else {
      Node *newLeaf = new Node;
      int virtualNode[ORDER + 1];
      for (int i = 0; i < ORDER; i++) {
        virtualNode[i] = cursor->keys[i];
      }
      int i = 0, j;
      while (x > virtualNode[i] && i < ORDER)
        i++;
      for (int j = ORDER + 1; j > i; j--) {
        virtualNode[j] = virtualNode[j - 1];
      }
      virtualNode[i] = x;
      newLeaf->IS_LEAF = true;
      cursor->size = (ORDER + 1) / 2;
      newLeaf->size = ORDER + 1 - (ORDER + 1) / 2;
      cursor->ptr[cursor->size] = newLeaf;
      newLeaf->ptr[newLeaf->size] = cursor->ptr[ORDER];
      cursor->ptr[ORDER] = NULL;
      for (i = 0; i < cursor->size; i++) {
        cursor->keys[i] = virtualNode[i];
      }
      for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
        newLeaf->keys[i] = virtualNode[j];
      }
      if (cursor == root) {
        Node *newRoot = new Node;
        newRoot->keys[0] = newLeaf->keys[0];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;
      } else {
        insertInternal(newLeaf->keys[0], parent, newLeaf);
      }
    }
  }
}
void BPTree::insertInternal(int x, Node *cursor, Node *child) {
  if (cursor->size < ORDER) {
    int i = 0;
    while (x > cursor->keys[i] && i < cursor->size)
      i++;
    for (int j = cursor->size; j > i; j--) {
      cursor->keys[j] = cursor->keys[j - 1];
    }
    for (int j = cursor->size + 1; j > i + 1; j--) {
      cursor->ptr[j] = cursor->ptr[j - 1];
    }
    cursor->keys[i] = x;
    cursor->size++;
    cursor->ptr[i + 1] = child;
  } else {
    Node *newInternal = new Node;
    int virtualKey[ORDER + 1];
    Node *virtualPtr[ORDER + 2];
    for (int i = 0; i < ORDER; i++) {
      virtualKey[i] = cursor->keys[i];
    }
    for (int i = 0; i < ORDER + 1; i++) {
      virtualPtr[i] = cursor->ptr[i];
    }
    int i = 0, j;
    while (x > virtualKey[i] && i < ORDER)
      i++;
    for (int j = ORDER; j > i; j--) {
      virtualKey[j] = virtualKey[j - 1];
    }
    virtualKey[i] = x;
    for (int j = ORDER + 1; j > i + 1; j--) {
      virtualPtr[j] = virtualPtr[j - 1];
    }
    virtualPtr[i + 1] = child;
    newInternal->IS_LEAF = false;
    cursor->size = (ORDER + 1) / 2;
    newInternal->size = ORDER + 1 - (ORDER + 1) / 2;
    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {
      newInternal->keys[i] = virtualKey[j];
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
      newInternal->ptr[i] = virtualPtr[j];
    }
    if (cursor == root) {
      Node *newRoot = new Node;
      newRoot->keys[0] = cursor->keys[cursor->size];
      newRoot->ptr[0] = cursor;
      newRoot->ptr[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
    } else {
      insertInternal(cursor->keys[cursor->size], findParent(root, cursor), newInternal);
    }
  }
}
Node *BPTree::findParent(Node *cursor, Node *child) {
  Node *parent;
  if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF) {
    return NULL;
  }
  for (int i = 0; i < cursor->size + 1; i++) {
    if (cursor->ptr[i] == child) {
      parent = cursor;
      return parent;
    } else {
      parent = findParent(cursor->ptr[i], child);
      if (parent != NULL)
        return parent;
    }
  }
  return parent;
}
void BPTree::display() {
    if (root == NULL) {
        cout << "Tree is empty" << endl;
        return;
    }
    int paddingcnt = 12;
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
      int size = q.size();
      string space(paddingcnt,' ');
      cout<<space;
      paddingcnt-=4;
      for (int i = 0; i < size; i++) {
        Node* node = q.front();
        q.pop();
        for (int j = 0; j < node->size; j++) {
            cout << node->keys[j] << " ";
        }
        if (!node->IS_LEAF) {
            for (int j = 0; j <= node->size; j++) {
                if (node->ptr[j] != NULL) {
                    q.push(node->ptr[j]);
                }
            }
        }
        cout << ", ";
        }
        cout << endl;
    }
}

Node *BPTree::getRoot() {
  return root;
}