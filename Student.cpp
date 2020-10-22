#include "Student.h"

bool cmp_rating(StudentNode& a, StudentNode& b) {
    return b.rating < a.rating;
}

bool cmp_group(StudentNode& a, StudentNode& b) {
    return a.group < b.group;
}

bool cmp_name(StudentNode& a, StudentNode& b) {
    return a.name < b.name;
}

void Student::Add(StudentNode S) {
    if (GroupNameList[S.group] == nullptr) {
        GroupNameList[S.group] = new RBTree<string, StudentNode*>();
    }
    StudentNode* p = new StudentNode(S);
    GroupNameList[S.group]->Insert(S.name, p);

    if (GroupRatingList[S.group] == nullptr) {
        GroupRatingList[S.group] = new Tree<double, StudentNode*>();
    }
    GroupRatingList[S.group]->Insert(S.rating, p);
}

void Student::Remove(StudentNode S) {
    GroupNameList[S.group]->deleteValue(S.name);
    GroupRatingList[S.group]->Erase(S.rating, &S);
}

bool check(std::string &str, std::string& name) {
    for (int i = 0; i < (int)str.size(); i++) {
        if (str[i] != name[i]) {
            return str[i] == '*';
        }
    }
    return name[str.size()] == 0;
}

void Student::Selection(std::list<Lex, std::allocator<Lex>>::iterator &p) {
    for (auto& P : GroupNameList.GetAll()) {
        for (auto& v : P.second->GetAll()) {
            std::cout << v->select << '\n';
            auto it = p;
            while (it->t_lex != LEX_END) {
                if (it->t_lex == LEX_RATING) {
                    std::cout << it->vmin << ' ' << it->vmax << '\n';
                    v->select &= (it->vmin <= v->rating && v->rating <= it->vmax);
                }
                if (it->t_lex == LEX_GROUP) {
                    std::cout << it->vmin << ' ' << it->vmax << '\n';
                    v->select &= (it->vmin <= v->group && v->group <= it->vmax);
                }
                if (it->t_lex == LEX_NAME) {
                    v->select &= check(it->str, v->name);
                }
                it++;
            }
        }
    }
}

void Student::Process(const std::string &buf, std::ostringstream& out){
    std::cout << "START\n";
    std::list<struct Lex> lst{Lex()};
    parse_string(buf, lst);
    auto p = lst.begin();
    ++p;
    for (; p != lst.end(); ++p){
        if (p->t_lex == LEX_INSERT) {
            ++p;
            Add(StudentNode(p->str));
        }
        if (p->t_lex == LEX_REMOVE){
            ++p;
            Remove(StudentNode(p->str));
        }
        if (p->t_lex == LEX_SELECT){
            for (auto& P: GroupNameList.GetAll()) {
                for (auto v : P.second->GetAll()) {
                    v->select = true;
                }
            }
            ++p;
            Selection(p);

        }
        if (p->t_lex == LEX_RESELECT){
            Selection(++p);
        }
        if (p->t_lex == LEX_PRINT){
            Print(++p, out);
        }
    }
    std::cout << "FINISH\n";
}

void Student::Print(std::list<struct Lex>::iterator &p, std::ostringstream& out) {
    bool prating = false;
    bool pname = false;
    bool pgroup = false;
    bool sort = false;
    Lex sort_name;
    while (p->t_lex != LEX_END){
        switch (p->t_lex) {
            case LEX_RATING:
                prating = true;
                break;
            case LEX_GROUP:
                pgroup = true;
                break;
            case LEX_NAME:
                pname = true;
                break;
            case LEX_SORT:
                sort = true;
                ++p;
                sort_name = Lex(p->t_lex);
                break;
            default:
                break;

        }
        ++p;
    }

    std::vector <StudentNode> arr;

    for (auto& P : GroupNameList.GetAll()) {
        for (auto v : P.second->GetAll()) {
            if (v->select)
                arr.push_back(*v);
        }
    }
    if (sort){
        switch (sort_name.t_lex){
            case LEX_RATING:
                std::sort(arr.begin(), arr.end(), cmp_rating);
                break;
            case LEX_GROUP:
                std::sort(arr.begin(), arr.end(), cmp_group);
                break;
            case LEX_NAME:
                std::sort(arr.begin(), arr.end(), cmp_name);
                break;;
            default:
                break;
        }
    }
    for (auto &v : arr) {
        v.Print(pname, pgroup, prating, out);
    }
}
