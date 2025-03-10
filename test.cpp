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

int main() {
    char *s1 = "BD",*s2 = "BC",*s3 = "AB";
    cout << s1 << " 23" << s2 << " " << s3;
    if (strcmp(s1,s2) > 0) swap(s1,s2);
    cout << s1 << " " << s2 << " " << s3 <<"\n";
    if (strcmp(s2,s3) > 0) swap(s2,s3);
    cout << s1 << " " << s2 << " " << s3 << "\n";
    if (strcmp(s1,s2) > 0) swap(s1,s2);
    cout << s1 << " " << s2 << " " << s3;
}   

/*
1
-G--
---E
12 2
-G--
-G--
*/