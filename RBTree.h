#include <iostream>
#include <vector>

enum Color {RED, BLACK, DOUBLE_BLACK};

template <typename Key, typename Val>
struct Node
{
    Key key;
    Val val;
    int color;
    Node *left, *right, *parent;

    explicit Node(Key k, Val v = Val());

    void GetAll(std::vector<Val>& res) {
        if (left) {
            left->GetAll(res);
        }
        res.push_back(val);
        if (right) {
            right->GetAll(res);
        }
    }
};

template <typename Key, typename Val>
class RBTree
{
private:
    Node<Key, Val> *root;
    int size;
protected:
    void rotateLeft(Node<Key, Val> *&);
    void rotateRight(Node<Key, Val> *&);
    void fixInsertRBTree(Node<Key, Val> *&);
    void fixDeleteRBTree(Node<Key, Val> *&);
    int getColor(Node<Key, Val> *&);
    void setColor(Node<Key, Val> *&, int);
    Node<Key, Val>* minValueNode(Node<Key, Val> *&);
    Node<Key, Val>* insertBST(Node<Key, Val> *&, Node<Key, Val> *&);
    Node<Key, Val>* deleteBST(Node<Key, Val> *&, Key k);
    Node<Key, Val>* Get(Node<Key, Val> *&, Key k);
    void ClearRB(Node<Key, Val> *&);
public:
    RBTree();
    void insertValue(Key);
    void deleteValue(Key);
    void Insert(Key, Val);
    Val& operator[](Key k);
    std::vector<Val> GetAll();
    ~RBTree();
};