#include "spellcheck.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>

template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

Corpus tokenize(const std::string& source) {
  /* TODO: Implement this method */
  Corpus tokens;
  std::vector <std::string::const_iterator> vec = find_all(source.begin(),source.end(),::isspace);
  return Corpus(tokens);
}

std::set<Mispelling> spellcheck(const Corpus& source, const Dictionary& dictionary) {
  /* TODO: Implement this method */
  return std::set<Mispelling>();
};

/* Helper methods */

#include "utils.cpp"