#include "HashTable.h"

template <typename Key, typename Value>
HashNode<Key, Value>& HashTable<Key, Value>::Get(Key k) {
    if (d == MaxD) {
        Rebuild();
    }
    int x = Hash(k);
    x %= size;
    for (HashNode<Key, Value> &h : H[x]) {
    if (h.k == k) {
    return h;
    }
    }
    H[x].push_back(HashNode<Key, Value>(k));
    d = std::max(d, (int)H[x].size());
    return H[x].back();
}

template<typename Key, typename Value>
Value& HashTable<Key, Value>::operator[](Key k) {
    HashNode<Key, Value>& T = Get(k);
    return T.v;
}

template<typename Key, typename Value>
void HashTable<Key, Value>::Rebuild() {
    vector<list<HashNode<Key, Value> > >B;
    int new_size = size * 2;
    B.resize(size * 2, list<HashNode<Key, Value> >());
    for (int i = 0; i < size; i++) {
        for (HashNode<Key, Value> &p : H[i]) {
            B[Hash(p.k) % new_size].push_back(p);
        }
        H[i].clear();
    }
    H = B;
    size = new_size;
    d = 0;
    for (int i = 0; i < size; i++)
        d = max(d, (int)H[i].size());
    B.clear();
}

template <typename Key, typename Value>
std::vector<std::pair<Key, Value> > HashTable<Key, Value>::GetAll() {
    std::vector<std::pair<Key, Value> > res;
    for (auto& p : H) {
        for (auto it : p) {
            res.push_back({it.k, it.v});
        }
    }
    return res;
}

template <typename Key, typename Value>
void HashTable<Key, Value>::Erase(Key k) {
    if (d == MaxD) {
        Rebuild();
    }
    int x = Hash(k);
    x %= size;
    auto it = H[x].begin();
    while (it != H[x].end()) {
        if ((*it).k == k) {
            H[x].erase(it);
            return;
        }
        it++;
    }
}
