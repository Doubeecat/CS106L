#include "class.h"
using namespace std;


Class::Class() {}

Class::Class(const vector <string> members) {
    for (auto x : members) names.insert(x);
}

const double Class::getData(const string name) {
    if (names.find(name) == names.end()) {
        return -1;
    }
    return score[name];
}

void Class::setData(const string name,const double sc) {
    if (names.find(name) == names.end()) {
        return ;
    }
    score[name] = sc;
}