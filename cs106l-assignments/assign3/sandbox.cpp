/*
 * CS106L Assignment 3: Make a Class
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 */

#include "class.h"
using namespace std;

void sandbox() {
  // STUDENT TODO: Construct an instance of your class!
  vector <string> name = {"Xiaoming","David"};
  Class ca(name);
  ca.setData("Xiaoming",13.0);
  cout << ca.getData("Xiaoming") << "\n";
  ca.setData("David",3.0);
  cout << ca.getData("David") << "\n";
  cout << ca.getData("cat") << "\n";
  return ;
}