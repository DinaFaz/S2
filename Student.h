#ifndef UNTITLED_STUDENT_H
#define UNTITLED_STUDENT_H

#include "RBTree.cpp"
#include "Tree.cpp"
#include "HashTable.cpp"
#include "Lex.h"
#include <string.h>
#include <math.h>
#include <sstream>
#include <algorithm>

#define eps 1e-6

struct StudentNode {
    std::string name;
    int group;
    double rating;
    char* info;
    bool select;

    StudentNode() {
        info = nullptr;
        group = -1;
        rating = 0;
        select = false;
    }

    StudentNode(const char* a, int gr, double r, char* p) {
        while (*a) {
            name += *a;
            a++;
        }
        group = gr;
        rating = r;
        info = p;
        select = false;
    }


    StudentNode(std::string a, int gr, double r, char* p) {
        name = a;
        group = gr;
        rating = r;
        info = p;
        select = false;
    }

    StudentNode(const std::string& s) {
        std::istringstream in(s);
        if (!(in >> name >> group >> rating)) {
            throw "Error: wrong arguments. Try the following:\n COMMAND [NAME] [GROUP] [RATING]\n";
        }
    }

    bool operator ==(StudentNode p) {
        return name == p.name && group == p.group &&
                fabs(rating - p.rating) < eps;
    }

    void Print(bool pname, bool pgroup, bool prating, std::ostringstream& out) {
        out << "{ ";
        if (pname)
            out << "name:" << name << "; ";
        if (pgroup)
            out << "group:" << group <<"; ";
        if (prating)
            out << "rating:" << rating << "; ";
        out << "}\n";
    }
};

class Student {
private:
    HashTable<int, RBTree<string, StudentNode*>*> GroupNameList;
    HashTable<int, Tree<double, StudentNode*>*> GroupRatingList;
public:
    void Process(const std::string& buf, std::ostringstream& out);
private:
    void Add(StudentNode S);
    void Remove(StudentNode S);
    void Selection(std::list<Lex, std::allocator<Lex> >::iterator &p);
    void Print(std::list<struct Lex>::iterator &p, std::ostringstream& out);
};

#endif //UNTITLED_STUDENT_H
