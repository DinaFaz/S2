#include "RBTree.h"

using namespace std;

template <typename Key, typename Val>
Node<Key, Val>::Node(Key k, Val v) {
    key = k;
    val = v;
    color = RED;
    left = right = parent = nullptr;
}

template <typename Key, typename Val>
RBTree<Key, Val>::RBTree() {
    root = nullptr;
    size = 0;
}

template <typename Key, typename Val>
int RBTree<Key, Val>::getColor(Node<Key, Val> *&node) {
    if (node == nullptr)
        return BLACK;

    return node->color;
}

template <typename Key, typename Val>
void RBTree<Key, Val>::setColor(Node<Key, Val> *&node, int color) {
    if (node == nullptr)
        return;

    node->color = color;
}

template <typename Key, typename Val>
Node<Key, Val>* RBTree<Key, Val>::insertBST(Node<Key, Val> *&root, Node<Key, Val> *&ptr) {
    if (root == nullptr)
        return ptr;

    if (ptr->key < root->key) {
        root->left = insertBST(root->left, ptr);
        root->left->parent = root;
    } else {
        root->right = insertBST(root->right, ptr);
        root->right->parent = root;
    }

    return root;
}

template <typename Key, typename Val>
void RBTree<Key, Val>::insertValue(Key k) {
    Node<Key, Val>* node = new Node<Key, Val>(k);
    root = insertBST(root, node);
    fixInsertRBTree(node);
    size++;
}

template <typename Key, typename Val>
void RBTree<Key, Val>::rotateLeft(Node<Key, Val> *&ptr) {
    Node<Key, Val> *right_child = ptr->right;
    ptr->right = right_child->left;

    if (ptr->right != nullptr)
        ptr->right->parent = ptr;

    right_child->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = right_child;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = right_child;
    else
        ptr->parent->right = right_child;

    right_child->left = ptr;
    ptr->parent = right_child;
}

template <typename Key, typename Val>
void RBTree<Key, Val>::rotateRight(Node<Key, Val> *&ptr) {
    Node<Key, Val> *left_child = ptr->left;
    ptr->left = left_child->right;

    if (ptr->left != nullptr)
        ptr->left->parent = ptr;

    left_child->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = left_child;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = left_child;
    else
        ptr->parent->right = left_child;

    left_child->right = ptr;
    ptr->parent = left_child;
}

template <typename Key, typename Val>
void RBTree<Key, Val>::fixInsertRBTree(Node<Key, Val> *&ptr) {
    Node<Key, Val> *parent = nullptr;
    Node<Key, Val> *grandparent = nullptr;
    while (ptr != root && getColor(ptr) == RED && getColor(ptr->parent) == RED) {
        parent = ptr->parent;
        grandparent = parent->parent;
        if (parent == grandparent->left) {
            Node<Key, Val> *uncle = grandparent->right;
            if (getColor(uncle) == RED) {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                ptr = grandparent;
            } else {
                if (ptr == parent->right) {
                    rotateLeft(parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                rotateRight(grandparent);
                std::swap(parent->color, grandparent->color);
                ptr = parent;
            }
        } else {
            Node<Key, Val> *uncle = grandparent->left;
            if (getColor(uncle) == RED) {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                ptr = grandparent;
            } else {
                if (ptr == parent->left) {
                    rotateRight(parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                rotateLeft(grandparent);
                swap(parent->color, grandparent->color);
                ptr = parent;
            }
        }
    }
    setColor(root, BLACK);
}

template <typename Key, typename Val>
void RBTree<Key, Val>::fixDeleteRBTree(Node<Key, Val> *&node) {
    if (node == nullptr)
        return;

    if (node == root) {
        root = nullptr;
        return;
    }

    if (getColor(node) == RED || getColor(node->left) == RED || getColor(node->right) == RED) {
        Node<Key, Val> *child = node->left != nullptr ? node->left : node->right;

        if (node == node->parent->left) {
            node->parent->left = child;
            if (child != nullptr)
                child->parent = node->parent;
            setColor(child, BLACK);
            delete (node);
        } else {
            node->parent->right = child;
            if (child != nullptr)
                child->parent = node->parent;
            setColor(child, BLACK);
            delete (node);
        }
    } else {
        Node<Key, Val> *sibling = nullptr;
        Node<Key, Val> *parent = nullptr;
        Node<Key, Val> *ptr = node;
        setColor(ptr, DOUBLE_BLACK);
        while (ptr != root && getColor(ptr) == DOUBLE_BLACK) {
            parent = ptr->parent;
            if (ptr == parent->left) {
                sibling = parent->right;
                if (getColor(sibling) == RED) {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rotateLeft(parent);
                } else {
                    if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
                        setColor(sibling, RED);
                        if(getColor(parent) == RED)
                            setColor(parent, BLACK);
                        else
                            setColor(parent, DOUBLE_BLACK);
                        ptr = parent;
                    } else {
                        if (getColor(sibling->right) == BLACK) {
                            setColor(sibling->left, BLACK);
                            setColor(sibling, RED);
                            rotateRight(sibling);
                            sibling = parent->right;
                        }
                        setColor(sibling, parent->color);
                        setColor(parent, BLACK);
                        setColor(sibling->right, BLACK);
                        rotateLeft(parent);
                        break;
                    }
                }
            } else {
                sibling = parent->left;
                if (getColor(sibling) == RED) {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rotateRight(parent);
                } else {
                    if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
                        setColor(sibling, RED);
                        if (getColor(parent) == RED)
                            setColor(parent, BLACK);
                        else
                            setColor(parent, DOUBLE_BLACK);
                        ptr = parent;
                    } else {
                        if (getColor(sibling->left) == BLACK) {
                            setColor(sibling->right, BLACK);
                            setColor(sibling, RED);
                            rotateLeft(sibling);
                            sibling = parent->left;
                        }
                        setColor(sibling, parent->color);
                        setColor(parent, BLACK);
                        setColor(sibling->left, BLACK);
                        rotateRight(parent);
                        break;
                    }
                }
            }
        }
        if (node == node->parent->left)
            node->parent->left = nullptr;
        else
            node->parent->right = nullptr;
        delete(node);
        setColor(root, BLACK);
    }
}

template <typename Key, typename Val>
Node<Key, Val>* RBTree<Key, Val>::deleteBST(Node<Key, Val> *&root, Key k) {
    if (root == nullptr)
        return root;

    if (k < root->key)
        return deleteBST(root->left, k);

    if (k > root->key)
        return deleteBST(root->right, k);

    if (root->left == nullptr || root->right == nullptr)
        return root;

    Node<Key, Val>* temp = minValueNode(root->right);
    root->key = temp->key;
    return deleteBST(root->right, temp->key);
}

template <typename Key, typename Val>
void RBTree<Key, Val>::deleteValue(Key k) {
    Node<Key, Val> *node = deleteBST(root, k);
    fixDeleteRBTree(node);
    size++;
}

template <typename Key, typename Val>
Node<Key, Val>* RBTree<Key, Val>::minValueNode(Node<Key, Val> *&node) {

    Node<Key, Val> *ptr = node;

    while (ptr->left != nullptr)
        ptr = ptr->left;
    return ptr;
}

template <typename Key, typename Val>
Node<Key, Val>* RBTree<Key, Val>::Get(Node<Key, Val>* &node, Key k) {
    if (node == nullptr || node->key == k) {
        return node;
    } else if (node->key < k) {
        return Get(node->right, k);
    } else {
        return Get(node->left, k);
    }
}

template <typename Key, typename Val>
void RBTree<Key, Val>::Insert(Key k, Val V) {
    Node<Key, Val>* p = Get(root, k);
    if (p == nullptr) {
        insertValue(k);
        p = Get(root, k);
    }
    p->val = V;
}

template <typename Key, typename Val>
Val& RBTree<Key, Val>::operator[](Key k) {
    Node<Key, Val>* p = Get(root, k);
    if (p == nullptr) {
        insertValue(k);
        p = Get(root, k);
    }
    return p->val;
}



template <typename Key, typename Val>
std::vector<Val> RBTree<Key, Val>::GetAll() {
    std::vector<Val> res;
    root->GetAll(res);
    return res;
}

template <typename Key, typename Val>
void RBTree<Key, Val>::ClearRB(Node<Key, Val>* &T) {
    if (T == nullptr)
        return ;
    ClearRB(T->left);
    ClearRB(T->right);
    delete T;
}

template <typename Key, typename Val>
RBTree<Key, Val>::~RBTree() {
    ClearRB(root);
    size = 0;
}
