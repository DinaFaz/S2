
#ifndef UNTITLED_HASHTABLE_H
#define UNTITLED_HASHTABLE_H

#include <iostream>
#include <vector>
#include <list>

using namespace std;

template <typename Key, typename Value>
struct HashNode {
    Key k;
    Value v;
    HashNode(Key k): k(k), v() {};
};

template <typename Key, typename Value>
class HashTable {
private:
    const static int MaxD = 4;
    vector<list< HashNode<Key, Value> > > H;
    int size;
    int d;

private:
    int Hash(const char* s) {
        const static int P = 239;
        int res = 0;
        const char* c = s;
        while (*c != 0) {
            res *= P;
            res += *c;
            c++;
        }
        return abs(res);
    }

    int Hash(string s) {
        const static int P = 199;
        int res = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            res *= P;
            res += s[i];
        }
        return abs(res);
    }

    int Hash(pair <int, int> p) {
        int res = p.first * (1000 * 1000 * 1000 + 7) + p.second;
        return abs(res);
    }

    int Hash(int a) {
        return a;
    }

public:
    HashTable() {
        size = 1;
        H = vector< list<HashNode<Key, Value> > >(size, list<HashNode<Key, Value> > ());
        d = 0;
    }

    HashNode<Key, Value>& Get(Key k);
    Value& operator[](Key k);
    void Rebuild();
    void Erase(Key k);
    std::vector<std::pair<Key, Value> > GetAll();

    ~HashTable() {
        for (int i = 0; i < (int)H.size(); i++) {
            H[i].clear();
        }
        H.clear();
    }
};

#endif //UNTITLED_HASHTABLE_H