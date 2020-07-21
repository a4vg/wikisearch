#ifndef PREPROCESSOR_CPP
#define PREPROCESSOR_CPP

#include "include/Preprocessor.h"

#include <sstream> // stringstream
#include <cstring> // c_str
#include <fstream> // ifstream, ofstream
#include <cctype> // tolower
#include <vector>
#include <cstdlib> // getenv
#include <iostream> // cerr

std::map<std::string, std::string> Preprocessor::withoutTilde = {
    {"ä", "a"},
    {"á", "a"},
    {"é", "e"},
    {"ë", "e"},
    {"í", "i"},
    {"ï", "i"},
    {"ó", "o"},
    {"ö", "o"},
    {"ú", "u"},
    {"ü", "u"},
    {"ñ", "n"},
    {"Á", "A"},
    {"Å", "A"},
    {"Ă", "A"},
    {"Ä", "A"},
    {"É", "E"},
    {"Ë", "E"},
    {"Í", "I"},
    {"Ï", "I"},
    {"Ó", "O"},
    {"Ö", "O"},
    {"Ú", "U"},
    {"Ü", "U"},
    {"Ñ", "N"}
  };

Preprocessor::Preprocessor
(std::string _text, std::string sw_filename, std::string lemmadic_filename): text(_text)
{
  const char *data_dir = std::getenv("DATA_DIR");
  if ((sw_filename.empty() || lemmadic_filename.empty()) && !data_dir)
  {
    std::cerr << "Please, set the env variable 'DATA_DIR' or provide a path for stopwords and lemmatization files.\nex: export DATA_DIR=./data";
    exit(1);
  }

  if (sw_filename.empty())
    sw_filename = std::string(data_dir) + "/stopwords/stopwords-es.txt";
  if (lemmadic_filename.empty())
    lemmadic_filename = std::string(data_dir) + "/lemma-dictionaries/lemmatization-es.txt";
  loadStopwords(sw_filename);
  loadLemmatizerDic(lemmadic_filename);
};

bool Preprocessor::isLetter(int c)
{
  // If c<0 is not ascii. Could be a latin character
  return (c<0) || ('a'<=c && c<='z') || ('A'<=c && c<='Z');
}

bool Preprocessor::hasTilde(int c)
{
  // A vowel with tilde is represented by two chars
  // The first char is -61
  return c==-61;
}

void Preprocessor::loadLemmatizerDic(std::string lem_filename)
{
  std::ifstream lemmadic_file(lem_filename);
  if (!lemmadic_file.is_open())
    throw "Can not open lemmatizer file: " + lem_filename;

  std::string line;

  std::string initword;
  std::string lemmaword;
  while (lemmadic_file >> lemmaword >> initword)
    this->lemmadic.insert(initword, lemmaword);
  lemmadic_file.close();
}

void Preprocessor::loadStopwords(std::string sw_filename)
{
  std::ifstream sw_file(sw_filename);
  if (!sw_file.is_open())
    throw "Can not open stopword file: " + sw_filename;

  std::string sw;
  while (sw_file >> sw)
    this->stopwords.insert(sw, true);
}

void Preprocessor::removeNonAscii(std::string &word)
{
  for (char &c: word)
    if (!isalpha(c))
    {
      word = "";
      return;
    }
}

void Preprocessor::removeStopword(std::string &word)
{
  if (word.empty()) return;

  if (this->stopwords.getKeyOf(word))
    word = ""; // is a stopword
}

void Preprocessor::lowerStr(std::string &word)
{
  if (word.empty()) return;

  for (char &c: word)
    c = (char)tolower(c);
}

void Preprocessor::tokenize(std::string &word)
{
  if (word.empty()) return;

  // Erase non alphabetic characters
  std::string::iterator itbegin = word.begin();
  std::string::iterator itend = word.end();
  while ((itbegin < itend) && (!this->isLetter(*itbegin) || !this->isLetter(*itend)))
  {
    if (!this->isLetter(*itbegin)) ++itbegin;
    if (!this->isLetter(*itend)) --itend;
  }

  // Trim word if necessary and send to stream
  bool hasChanged = itbegin != word.begin() || itend != word.end()-1;
  if (!hasChanged)
    return;
  if (itbegin!=itend)
    word = std::string(itbegin, itend+1);
  else word = "";
}

void Preprocessor::lemmatize(std::string &word)
{
  if (word.empty()) return;

  std::string lemmaword = this->lemmadic.getKeyOf(word);
  if (!lemmaword.empty())
    word = lemmaword;
  // word not available
}

void Preprocessor::removeTilde(std::string &word)
{
  if (word.empty()) return;
  for (int i=0; i<word.length(); ++i)
  {
    if (Preprocessor::hasTilde(word[i]))
    { // Vowel with tilde is represented by two chars or a string.
      // string(it, it+2) builds a string with this two chars and
      // ++it skips the second char

      std::string newword = word.substr(0, i);
      newword += Preprocessor::withoutTilde[word.substr(i, 2)];
      newword += word.substr(i+2, word.length()-i-2);
      word = newword;
      // break; // uncomment to assume there's just one tilde 
    }
  }
}

void Preprocessor::setText(std::string _text)
{
  this->text = _text;
  this->isPreprocessed = false;
  this->wordCount.clear();
}

std::map<std::string, int> Preprocessor::getWordCount()
{
  if (!this->isPreprocessed)
    this->preprocess();
  return this->wordCount;
}

void Preprocessor::exportWordCount(std::string filename)
{
  if (!this->isPreprocessed)
    this->preprocess();

  std::ofstream outfile(filename);
  for (const auto& pair: this->wordCount)
    // pair.first = string, pair.second = int
    outfile << pair.first << " " << pair.second << "\n";
}

void Preprocessor::preprocess()
{
  // Read infile word by word
  std::stringstream stream(this->text);
  std::string word;
  while(stream >> word)
  {
    this->tokenize(word);
    this->lowerStr(word);
    this->lemmatize(word);
    this->removeStopword(word);
    Preprocessor::removeTilde(word);
    this->removeNonAscii(word);

    if (!word.empty())
    {
      if (!this->wordCount.count(word))
        this->wordCount[word] = 1;
      else // repeated word
        this->wordCount[word] += 1;
    }
  }
  this->isPreprocessed = true;
}

#endif