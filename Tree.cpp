#include "Tree.h"


template<typename  Key, typename Val>
Tree<Key, Val>::Tree() {
    T = nullptr;
    size = 0;
}

template<typename Key, typename Val>
static void ClearN(TreeNode<Key, Val>* T) {
    if (T == nullptr)
        return ;
    ClearN(T->L);
    ClearN(T->R);
    delete T;
}

template<typename Key, typename Val>
void Tree<Key, Val>::Clear() {
    ClearN(T);
    size = 0;
}

template<typename Key, typename Val>
Tree<Key, Val>::~Tree() {
    Clear();
}


template<typename Key, typename Val>
static TreeNode<Key, Val>* Merge(TreeNode<Key, Val>* R, TreeNode<Key, Val>* L) {
    if (L == nullptr)
        return R;
    if (R == nullptr)
        return L;
    R->L = Merge(R->L, L);
    return R;
}

template<typename Key, typename Val>
static TreeNode<Key, Val>* InsertN(TreeNode<Key, Val>* T, TreeNode<Key, Val>* S) {
    if (T == nullptr) {
        return S;
    }
    if (T->key < S->key) {
        T->R = InsertN(T->R, S);
    } else {
        T->L = InsertN(T->L, S);
    }
    return T;
}

template<typename Key, typename Val>
void Tree<Key, Val>::Insert(Key k, Val v) {
    size++;
    T = InsertN(T, new TreeNode<Key, Val>(k, v));
}


template<typename Key, typename Val>
static TreeNode<Key, Val>* EraseN(TreeNode<Key, Val>* T, Key k, Val v) {
    if (T->key == k && *T->val == *v) {
        TreeNode<Key, Val>* P = Merge(T->L, T->R);
        delete T;
        return P;
    }
    if (T->key > k)  {
        T->L = EraseN(T->L, k, v);
    } else {
        T->R = EraseN(T->R, k, v);
    }
    return T;
}

template<typename Key, typename Val>
void Tree<Key, Val>::Erase(Key k, Val v) {
    size--;
    T = EraseN(T, k, v);
}

template<typename Key, typename Val>
static TreeNode<Key, Val>* GetN(TreeNode<Key, Val>* T, Key x, Val v) {
    if (T == nullptr  || T->key == x)
        return T;
    if (T->key < x) {
        return GetN(T->R, x);
    } else {
        return GetN(T->L, x);
    }
}

template<typename Key, typename Val>
TreeNode<Key, Val>* Tree<Key, Val>::Get(Key k) {
    return GetN(T, k);
}

template<typename Key, typename Val>
Val& Tree<Key, Val>::operator[](Key k) {
    TreeNode<Key, Val>* p = Get(k);
    if (p == nullptr) {
        Insert(k);
        p = Get(k);
    }
    return p->val;
}

template <typename Key, typename Val>
std::vector<Val> Tree<Key, Val>::GetAll() {
    std::vector<Val> res;
    T->GetAll(res);
    return res;
}