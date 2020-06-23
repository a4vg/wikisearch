#include <iostream>
#include <vector>
#include <map>

template <typename T>
struct TrieNode {
  T key;
  std::map<char, TrieNode<T>*> children;

  TrieNode(T _key): key(_key){};
  ~TrieNode(){};
};

template <typename T>
class Trie {
  TrieNode<T> *root;
  int nwords;

  void getWords(TrieNode<T>* curnode, std::vector<std::string>& words, std::string prefix);
  public:
    Trie();
    bool empty();
    int getTotalWords();
    bool exists(std::string word);
    bool insert(std::string word, T key);
    void getWords(std::vector<std::string>& words);
};

template <typename T>
Trie<T>::Trie():
root(new TrieNode<T>(T{})), nwords(0){}

template <typename T>
bool Trie<T>::empty()
{
  return !this->nwords;
}

template <typename T>
int Trie<T>::getTotalWords()
{
  return nwords;
}

template <typename T>
bool Trie<T>::exists(std::string word)
{
  TrieNode<T> *curnode = this->root;
  for (const char& c: word)
  {
    if (!curnode->children.count(c))
      return false;
    curnode = curnode->children[c];
  }
  return true;
}

template <typename T>
bool Trie<T>::insert(std::string word, T key)
{
  TrieNode<T> *curnode = this->root;
  for (const char& c: word)
  {
    if (!curnode->children.count(c))
      curnode->children[c] = new TrieNode<T>(T{});
    curnode = curnode->children[c];
  }
  
  // Set key in last character to mark end of word
  // only if the key is empty
  if (curnode->key)
    return false; // word exists
  curnode->key = key;
  ++nwords;
  return true; // word inserted
}

template <typename T>
void Trie<T>::getWords(TrieNode<T>* curnode, std::vector<std::string>& words, std::string prefix)
{
  if (curnode->key) // leaf
  {
    words.push_back(prefix);
    return;
  }

  for (const auto& pair : curnode->children)
    // pair.first = letter, pair.second = node
    getWords(pair.second, words, prefix+pair.first);
}

template <typename T>
void Trie<T>::getWords(std::vector<std::string>& words)
{
  this->getWords(this->root, words, "");
}

int main(){
  std::vector<std::string> words =
  {
    "wikipedia",
    "espanol",
    "edicion",
    "espanol",
    "wikipedia",
    "igual",
    "version",
    "wikipedia",
    "existir",
    "idioma",
    "enciclopedia"
  };

  Trie<bool> trie;
  int repeated_count = 0;
  std::cout << "inserting words in vector:\n";
  for (auto& word : words)
  {
    bool repeated = !trie.insert(word, true);
    std::cout << word;
    if (repeated){
      ++repeated_count;
      std::cout << " - Repeated!";
    }
    std::cout << "\n";
  }

  std::vector<std::string> words_trie;
  trie.getWords(words_trie);
  std::cout << "\ntotal words inserted: " << trie.getTotalWords() << "\n";
  std::cout << repeated_count << " repeated words\n";
  std::cout << "words in trie:\n";
  for (auto& word : words_trie)
    std::cout << word << "\n";
}