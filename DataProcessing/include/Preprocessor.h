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
  std::map<std::string, std::string> withoutTilde = {
    {"á", "a"},
    {"é", "e"},
    {"í", "i"},
    {"ó", "o"},
    {"ú", "u"},
    {"ñ", "n"},
    {"Á", "A"},
    {"É", "E"},
    {"Í", "I"},
    {"Ó", "O"},
    {"Ú", "U"},
    {"Ñ", "N"}
  };

  bool isLetter(int c);
  bool hasTilde(int c);

  void loadLemmatizerDic(std::string lemmadic_filename);
  void loadStopwords(std::string sw_filename);

  void removeNonAscii(std::string &word);
  void removeStopword(std::string &word);
  void lowerStr(std::string &word);
  void tokenize(std::string &word);
  void lemmatize(std::string &word);
  void removeTilde(std::string &word);

  public:
    Preprocessor
    (std::string _text="", std::string sw_filename="", std::string lemmadic_filename="");
    ~Preprocessor(){};
    void setText(std::string _text);
    std::map<std::string, int> getWordCount();
    void exportWordCount(std::string filename);
    void preprocess();
};

// #endif