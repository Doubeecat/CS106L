/*
Undo the destiny.
*/
#include <bits/stdc++.h>
#include "fraction.h"
using namespace std;
#define ll long long
fraction fraction::operator + (const fraction &a) {
    int newbase = base * a.base;
    int newfrac = a.base * frac + base * a.frac;
    int g = __gcd(newbase,newfrac);
    return fraction(newbase/g,newfrac/g);
}
fraction fraction::operator + (const int &a) {
    int newbase = base;
    int newfrac = frac + a * base;
    int g = __gcd(newbase,newfrac);
    return fraction(newbase/g,newfrac/g);
}
fraction fraction::operator - (const fraction &a) {
    int newbase = base * a.base;
    int newfrac = a.base * frac - base * a.frac;
    int g = __gcd(newbase,newfrac);
    return fraction(newbase/g,newfrac/g);
}
fraction fraction::operator * (const fraction &a) {
    int newbase = base * a.base;
    int newfrac = frac * a.frac;
    int g = __gcd(newbase,newfrac);
    return fraction(newbase/g,newfrac/g);
}
fraction fraction::operator / (const fraction &a) {
    int newbase = base * a.frac;
    int newfrac = frac * a.base;
    int g = __gcd(newbase,newfrac);
    return fraction(newbase/g,newfrac/g);
}
fraction& fraction::operator += (const fraction &a) {
    
    frac = a.base * frac + base * a.frac;
    base *= a.base;
    int g = __gcd(base,frac);
    base /= g,frac /= g;
    return *this;
}
fraction& fraction::operator -= (const fraction &a) {
    
    frac = a.base * frac - base * a.frac;
    base *= a.base;
    int g = __gcd(base,frac);
    return *this;
}
fraction& fraction::operator = (const fraction &a){
    base = a.base;
    frac = a.frac;
    return *this;
}

ostream& operator << (ostream& os,const fraction &a) {
    os << "(" << a.frac << "/" << a.base << ")";
    return os;
}
template <class T,class Container = std::vector <T> > 
class Stack {
    public:
        Stack() = default;
        ~Stack() = default;
    private:
        vector <T> s;
}

int main() {
    fraction a(2,1),b(4,3),c(6,5);
    cout << a << " " << b <<  " " << c << "\n";
    cout << a + b;
    cout << b + c;
    cout << ((a += b) += c);
}
