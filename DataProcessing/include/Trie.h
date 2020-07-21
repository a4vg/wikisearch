#ifndef TRIE_H
#define TRIE_H

#include <map>
#include <vector>
#include "lib/json.hpp"

using json = nlohmann::json;

template <typename T>
struct TrieNode
{
  T key;
  std::map<char, TrieNode<T>*> children;

  TrieNode(T _key): key(_key){};
  ~TrieNode()
  {
    for(auto it = children.begin(); it != children.end(); ++it)
      delete it->second;
  };
};

template <typename T>
class Trie
{
  TrieNode<T> *root;
  int nwords;

  void getWords(TrieNode<T>* curnode, std::string prefix, std::vector<std::string>* pwords, std::ofstream* pfile=nullptr);
  json toJson(TrieNode<T>* curnode, bool lower);
  public:
    Trie();
    ~Trie();
    bool empty();
    int getTotalWords();
    bool insert(std::string word, T key);
    T getKeyOf(std::string word);
    void getWords(std::vector<std::string>& words);
    void exportWords(std::string filename);
    void toJson(json &jsonTrie, bool lower=true);
    void exportAsJson(std::string filename, bool lower=true);
};

#include "Trie.tpp"

#endif