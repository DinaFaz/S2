#ifndef UNTITLED_LEX_H
#define UNTITLED_LEX_H

#include <string>
#include <list>

enum type_of_lex{
    LEX_NULL,
    LEX_SELECT,LEX_INSERT,LEX_RESELECT,LEX_REMOVE,LEX_PRINT,LEX_SORT,
    LEX_END,
    LEX_NAME,LEX_GROUP, LEX_RATING, LEX_STRING
};

struct Lex {
    type_of_lex t_lex;
    double vmin;
    double vmax;
    std::string str;
    Lex():t_lex(LEX_NULL),vmin(0),vmax(0),str(""){}
    explicit Lex (type_of_lex t);
    Lex(type_of_lex t, double min, double max);
    Lex(type_of_lex t, const std::string &s);
};

void parse_string(const std::string &str, std::list<class Lex> &lst);

void add_diapason(const std::string &buf, double diapason[]);

#endif //UNTITLED_LEX_H
