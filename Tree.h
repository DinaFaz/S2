#include <vector>

template<typename Key, typename Val>
struct TreeNode {
    TreeNode<Key, Val>* L;

    TreeNode<Key, Val>* R;
    Key key;
    Val val;

    TreeNode() {
        L = R = nullptr;
    }

    TreeNode(Key k) {
        L = R = nullptr;
        key = k;
    }

    TreeNode(Key k, Val v) {
        L = R = nullptr;
        key = k;
        val = v;
    }

    void GetAll(std::vector<Val>& res) {
        if (L) {
            L->GetAll(res);
        }
        res.push_back(val);
        if (R) {
            R->GetAll(res);
        }
    }

    ~TreeNode() {};
};

template<typename Key, typename Val>
class Tree {
private:
    TreeNode<Key, Val>* T;
    int size;
public:
    Tree();
    void Clear();
    void Insert(Key k, Val v = Val());
    void Erase(Key k, Val v);
    Val& operator[](Key k);
    TreeNode<Key, Val>* Get(Key k);
    std::vector<Val> GetAll();
    ~Tree();
};

