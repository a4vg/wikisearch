// #ifndef PREPROCESSOR_H
// #define PREPROCESSOR_H

#include <string>
#include <map>

#include "Trie.h"

class Preprocessor
{
  std::string text;
  std::map<std::string, int> wordCount;
  bool isPreprocessed = false;

  Trie<bool> stopwords;
  Trie<std::string> lemmadic;
  static std::map<std::string, std::string> withoutTilde;

  bool isLetter(int c);
  static bool hasTilde(int c);

  void loadLemmatizerDic(std::string lemmadic_filename);
  void loadStopwords(std::string sw_filename);

  void removeNonAscii(std::string &word);
  void removeStopword(std::string &word);
  void lowerStr(std::string &word);
  void tokenize(std::string &word);
  void lemmatize(std::string &word);

  public:
    static void removeTilde(std::string &word);

    Preprocessor
    (std::string _text="", std::string sw_filename="", std::string lemmadic_filename="");
    ~Preprocessor(){};
    void setText(std::string _text);
    std::map<std::string, int> getWordCount();
    void exportWordCount(std::string filename);
    void preprocess();
};

// #endif