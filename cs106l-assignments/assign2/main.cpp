/*
 * CS106L Assignment 2: Marriage Pact
 * Created by Haven Whitney with modifications by Fabio Ibanez & Jacob Roberts-Baca.
 *
 * Welcome to Assignment 2 of CS106L! Please complete each STUDENT TODO
 * in this file. You do not need to modify any other files.
 *
 */

#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>
#include <sstream>

std::string kYourName = "Peter Zhang"; // Don't forget to change this!

/**
 * Takes in a file name and returns a set containing all of the applicant names as a set.
 *
 * @param filename  The name of the file to read.
 *                  Each line of the file will be a single applicant's name.
 * @returns         A set of all applicant names read from the file.
 *
 * @remark Feel free to change the return type of this function (and the function
 * below it) to use a `std::unordered_set` instead. If you do so, make sure
 * to also change the corresponding functions in `utils.h`.
 */
std::set<std::string> get_applicants(std::string filename) {
  // STUDENT TODO: Implement this function.
	std::ifstream inp(filename);
	std::set <std::string> applicants;
	if (inp.is_open()) {
		std::string name;
		while (getline(inp,name)) {
			applicants.insert(name);
		}
	} else {
		throw std::runtime_error("fail to open file");
	}
	return applicants;
}

/**
 * Takes in a set of student names by reference and returns a queue of names
 * that match the given student name.
 *
 * @param name      The returned queue of names should have the same initials as this name.
 * @param students  The set of student names.
 * @return          A queue containing pointers to each matching name.
 */
bool judge_same_initials(std::string a,std::string b) {
	return a[0] == b[0];
}
std::queue<const std::string*> find_matches(std::string name, std::set<std::string>& students) {
  // STUDENT TODO: Implement this function.
  std::stringstream inp(name);
//   inp << name;
  std::string fst,snd;
  inp >> fst >> snd;
  std::queue <const std::string*> q;
  for (const auto &x : students) {
	inp << x;
	std::string a,b;inp >> a >> b;
	if (judge_same_initials(a,name) && judge_same_initials(b,name)) {
		q.push(&x);
	}
  }
  return q;
}

/**
 * Takes in a queue of pointers to possible matches and determines the one true match!
 *
 * You can implement this function however you'd like, but try to do something a bit
 * more complicated than a simple `pop()`.
 *
 * @param matches The queue of possible matches.
 * @return        Your magical one true love.
 *                Will return "NO MATCHES FOUND." if `matches` is empty.
 */
std::string get_match(std::queue<const std::string*>& matches) {
  // STUDENT TODO: Implement this function.
  if (matches.empty()) {
	return "NO MATCHES FOUND.";
  }
  std::string ans = *matches.front();matches.pop();
  return ans;
}

// int main() {
// 	std::set <std::string> applicants = get_applicants("students.txt");
// 	for (auto x : applicants) std::cerr << x << "\n";
// }

/* #### Please don't remove this line! #### */
#include "autograder/utils.hpp"
