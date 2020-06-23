#include <iostream>
#include <string>
#include <cstring> // c_str
#include <vector>
#include <fstream> // ifstream, ofstream
#include <sstream> // stringstream
#include <cctype> // tolower
#include <map>

class Preprocessor {
  std::string infilename;
  std::string outfilename;
  std::vector<std::string> stopwords;
  std::map<std::string, std::string> lemmadic;
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
    (std::string _infilename, std::string _outfilename, std::string sw_filename="stopwords/stopwords-es.txt", std::string lemmadic_filename="lemma-dictionaries/lemmatization-es.txt");
    ~Preprocessor(){};
    void preprocess();
};

Preprocessor::Preprocessor
(std::string _infilename, std::string _outfilename, std::string sw_filename, std::string lemmadic_filename):
infilename(_infilename), outfilename(_outfilename)
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
  {
    lemmadic.insert(std::pair<std::string, std::string>(initword, lemmaword) );
  }
  lemmadic_file.close();
}

void Preprocessor::loadStopwords(std::string sw_filename)
{
  std::ifstream sw_file(sw_filename);
  if (!sw_file.is_open())
    throw "Can not open stopword file";

  std::string sw;
  while (getline(sw_file, sw))
    this->stopwords.push_back(sw);
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

  // Find word in stopwords with binary search
  int low=0, high=this->stopwords.size();
  while (low <= high)
  {
    int mid = (low+high)/2;
    if (word == this->stopwords[mid]){
      word = ""; // is a stopword, remove
      return;
    }
    if (word.compare(this->stopwords[mid]) < 0) 
      high = mid-1;
    else
      low = mid+1;
  }
  // is not a stopword
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

  if (lemmadic.count(word) == 0)
    return; // word not in map
  word = lemmadic[word];
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

void Preprocessor::preprocess() {
  // Open files
  std::ifstream infile(this->infilename);
  std::ofstream outfile(this->outfilename);

  if (!infile.is_open() || !outfile.is_open())
    throw "Can not open input or output file";

  // Read infile word by word
  std::string word;
  while(infile >> word)
  {
    this->tokenize(word);
    this->lowerStr(word);
    this->removeStopword(word);
    this->lemmatize(word);
    this->removeTilde(word);
    this->removeNonAscii(word);

    if (!word.empty())
      outfile << word << "\n";
  }

  // Close files
  infile.close();
  outfile.close();
}

int main()
{
  Preprocessor preprocessor("text-example.txt", "output.txt");
  preprocessor.preprocess();
}