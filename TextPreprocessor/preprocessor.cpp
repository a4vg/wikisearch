#include <iostream>
#include <cstring> // c_str
#include <fstream> // ifstream, ofstream
#include <sstream> // stringstream
#include <cctype> // tolower
#include <string>
#include <vector>
#include <map>

#include "Trie.cpp"

class Preprocessor {
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
    (std::string _text="", std::string sw_filename="stopwords/stopwords-es.txt", std::string lemmadic_filename="lemma-dictionaries/lemmatization-es.txt");
    ~Preprocessor(){};
    void setText(std::string _text);
    std::map<std::string, int> getWordCount();
    void exportWordCount(std::string filename);
    void preprocess();
};

Preprocessor::Preprocessor
(std::string _text, std::string sw_filename, std::string lemmadic_filename): text(_text)
{
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
    throw "Can not open stopword file";

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
    if (this->hasTilde(word[i]))
    { // Vowel with tilde is represented by two chars or a string.
      // string(it, it+2) builds a string with this two chars and
      // ++it skips the second char

      std::string newword = word.substr(0, i);
      newword += this->withoutTilde[word.substr(i, 2)];
      newword += word.substr(i+2, word.length()-i-2);
      word = newword;
      break; // assumes there's just one tilde 
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
    outfile << pair.first << " " << pair.second;
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
    this->removeTilde(word);
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

int main()
{
  std::string text1 = "Wikipedia en español es la edición en español de Wikipedia. Al igual que las versiones de Wikipedia que existen en otros idiomas, es una enciclopedia de contenido libre, publicada en Internet bajo las licencias libres CC BY-SA 3.0 y GFDL. En la actualidad cuenta con 1 606 871 artículos, y es escrita por usuarios voluntarios, es decir, que cualquiera puede editar un artículo, corregirlo o ampliarlo. Los servidores son administrados por la Fundación Wikimedia, una organización sin ánimo de lucro cuya financiación se basa fundamentalmente en donaciones.";
  std::string text2 = "Es por ello que este artículo tiene la intención de enseñaros a redactar un texto argumentativo desde el principio para que podáis aplicarlo posteriormente sin tener en cuenta la tesis que vayáis a defender. Si conseguimos que previamente a la redacción el alumno tenga por escrito un guión previo muy definido, la redacción será mucho más fácil. En este sentido el profesor es importante que no sólo valore el texto argumentativo, sino también el borrador o plantilla elaborada por el alumno.";
  
  std::cout << "Loading language files to preprocessor...\n";
  Preprocessor preprocessor(text1); // text can also be set with .setText(string)

  std::cout << "\nWord count for text 1:\n";
  auto wordCount = preprocessor.getWordCount();
  for (const auto& count: wordCount)
    std::cout << count.first << ": " << count.second << "\n";

  preprocessor.setText(text2);

  std::cout << "\nWord count for text 2:\n";
  wordCount = preprocessor.getWordCount();
  for (const auto& count: wordCount)
    std::cout << count.first << ": " << count.second << "\n";
  preprocessor.exportWordCount("output.txt");
}