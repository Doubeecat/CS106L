#include <vector>
#include <string>
#include <map>
#include <set>
using namespace std;
#ifndef _class_h
#define _class_h

class Class {
    public:
        Class();
        Class(const vector <string> members) ;
        const double getData(const string name);
        void setData(const string name,const double sc);
    private:
        set <string> names;
        map <string,double> score;
};

#endif