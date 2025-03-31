#include <string>
using namespace std;
#ifndef _account_h
#define _account_h

class account {
    public:
        account(string name,double val);
        void withdraw(double amount);
        void debug();
    private:
        string name;
        double val;
};

#endif