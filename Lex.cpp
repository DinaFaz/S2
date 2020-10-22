#include "Lex.h"
#include <string>
#include <list>
#include <climits>
#include <sstream>
#include <iostream>

void parse_string(const std::string &str, std::list<class Lex> &lst){
    std::stringstream ss(str);
    std::string param;
    if (ss >> param){
        bool flag = false;
        if (param == "select"){
            flag = true;
            lst.emplace_back(Lex(LEX_SELECT));
        }
        if (param == "reselect"){
            flag = true;
            lst.emplace_back(Lex(LEX_RESELECT));
        }
        if (flag){
            while (ss >> param){
                if (param.substr(0, 3) == "end"){
                    lst.emplace_back(Lex(LEX_END));
                    return;
                }
                if (param.substr(0, 6) == "rating") {
                    double diapason[] = {0, 0};
                    std::string buf = param.substr(7);
                    add_diapason(buf, diapason);
                    lst.emplace_back(Lex(LEX_RATING, diapason[0], diapason[1]));
                    continue;
                }
                if (param.substr(0, 5) == "group") {
                    double diapason[] = {0, 0};
                    std::string buf = param.substr(6);
                    add_diapason(buf, diapason);
                    lst.emplace_back(Lex(LEX_GROUP, diapason[0], diapason[1]));
                    continue;
                }
                if (param.substr(0, 4) == "name"){
                    lst.emplace_back(Lex(LEX_NAME, param.substr(5)));
                    continue;
                }
            }
            throw "Error: wrong arguments. Try the following:\n\
    {select | reselect} [ARG]=[DIAPASON]... end\n\
    where ARG = {group | rating | name}\n\
          DIAPASON - one or two numbers limiting the diapason for current ARG\n";
        }

        if (param == "print"){
            std::cout << "GO";
            lst.emplace_back(Lex(LEX_PRINT));
            while (ss >> param){
                if (param == "end"){
                    lst.emplace_back(Lex(LEX_END));
                    return;
                }
                if (param == "rating") {
                    lst.emplace_back(Lex(LEX_RATING));
                    continue;
                }
                if (param == "sort"){
                    lst.emplace_back(Lex(LEX_SORT));
                    continue;
                }
                if (param == "group") {
                    lst.emplace_back(Lex(LEX_GROUP));
                    continue;

                }
                if (param == "name"){
                    lst.emplace_back(Lex(LEX_NAME));
                    continue;
                }
            }
            throw "Error: wrong arguments. Try the following:\n\
    print [ARGS]  {sort ARG} end\n\
    where ARG = {group | rating | name}\n";
        }
        if (param == "insert"){
            lst.emplace_back(Lex(LEX_INSERT));
            std::string tmp;
            getline(ss, tmp);
            lst.emplace_back(Lex(LEX_STRING, tmp));
            return;
        }
        if (param == "remove"){
            lst.emplace_back(Lex(LEX_REMOVE));
            std::string tmp;
            getline(ss, tmp);
            lst.emplace_back(Lex(LEX_STRING, tmp));
            return;
        }
        throw "Error: wrong command. Try \"help\"";
    }
}

void add_diapason(const std::string &buf, double diapason[]) {
    if (buf.length() == 1 && buf[0] == '*'){
        diapason[0] = 0;
        diapason[1] = INT_MAX;
        return;
    }
    int i = 0;
    int cnt[2] = {-1, -1};
    for (auto c : buf){
        if (isdigit(c)){
            diapason[i] = diapason[i] * 10 + c - '0';
            if (cnt[i] != -1)
                cnt[i]++;
            continue;
        }
        if (c == '*'){
            diapason[i] = -1;
            continue;
        }
        if (c == '.') {
            cnt[i] = 0;
        }
        if (c == '-'){
            i++;
            continue;
        }
    }
    for (int i = 0; i < 2; i++) {
        if (cnt[i] != -1) {
            while (cnt[i]) {
                diapason[i] /= 10.0;
                cnt[i]--;
            }
        }
    }

    if (i == 0) {
        diapason[1] = diapason[0];
    }
    if (diapason[0] == -1){
        diapason[0] = 0;
    }
    if (diapason[1] == -1){
        diapason[1] = INT_MAX;
    }
}

Lex::Lex(type_of_lex t) {
    t_lex = t;
    vmin = .0;
    vmax = .0;
    str = "";
}

Lex::Lex(type_of_lex t, double min, double max) {
    t_lex = t;
    vmin = min;
    vmax = max;
    str = "";
}

Lex::Lex(type_of_lex t, const std::string &s) {
    t_lex = t;
    vmin = .0;
    vmin = .0;
    str = s;
}