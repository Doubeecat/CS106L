/*
Undo the destiny.
*/
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define FO(x) {freopen(#x".in","r",stdin);freopen(#x".out","w",stdout);}
#define pii pair<int,int>
#define pll pair<ll,ll>
#define mp make_pair

void PrintStateBits(const istringstream &s) {
    cout << (s.bad() ? "B" : "-");
    cout << (s.good() ? "G" : "-");
    cout << (s.fail() ? "F" : "-");
    cout << (s.eof() ? "E" : "-");
    cout << endl;
}

int StringToInterger(const string &s) {
    int number;
    istringstream iss(s);
    PrintStateBits(iss);
    iss >> number;
    PrintStateBits(iss);
    return number;
}

void swap(char *s1,char *s2) {
    char t;
    t = *s1;
    *s1 = *s2;
    *s2 = t;
}

template <typename T>//先声明模板类型为 T
pair<T,T> my_minmax(T a,T b) {
    if (a < b) return {a,b};
    else return {b,a};
}

int main() {
pair <double,double> p1 = my_minmax(1.2,3.2);
cout << p1.first << p1.second;
pair <string,string> p2 = my_minmax("a","b");
cout << p2.first << p2.second;
}   

/*
1
-G--
---E
12 2
-G--
-G--
*/