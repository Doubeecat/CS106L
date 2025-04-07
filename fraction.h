#include <iostream>
using namespace std;
#ifndef _fraction_h
#define _fraction_h

class fraction {
    public:
        fraction(){};
        fraction(int a,int b):base(a),frac(b){};
        fraction operator + (const fraction &a);
        fraction operator + (const int &a);
        fraction operator - (const fraction &a);
        fraction operator * (const fraction &a);
        fraction operator / (const fraction &a);
        fraction& operator += (const fraction &a);
        fraction& operator -= (const fraction &a);
        fraction& operator = (const fraction &a);
        
    private:
        int base,frac;
        friend ostream& operator << (ostream &os,const fraction &a);
};


#endif