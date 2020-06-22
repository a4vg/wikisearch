#include <iostream>
#include <string>
#include <cstring> // c_str
#include <vector>
#include <fstream> // ifstream, ofstream
#include <sstream> // stringstream
#include <cctype> // tolower
#include "lemmagen/src/RdrLemmatizer.h" // lemmatizer

class Preprocessor {
  std::string infilename;
  std::string outfilename;
  std::vector<std::string> stopwords;
  RdrLemmatizer *lemmatizer = nullptr;

  bool isLetter(int c);
  void loadLemmatizerDic(std::string lemmadic_filename);
  void loadStopwords(std::string sw_filename);
  void removeStopword(std::string &word);
  void lowerStr(std::string &word);
  void tokenize(std::string &word);
  void lemmatize(std::string &word);

  public:
    Preprocessor
    (std::string _infilename, std::string _outfilename, std::string sw_filename="stopwords/stopwords-es.txt", std::string lemmadic_filename="lemmagen/dictionaries/spanish.bin");
    ~Preprocessor();
    void preprocess();
};

Preprocessor::Preprocessor
(std::string _infilename, std::string _outfilename, std::string sw_filename, std::string lemmadic_filename):
infilename(_infilename), outfilename(_outfilename)
{
  loadStopwords(sw_filename);
  loadLemmatizerDic(lemmadic_filename);
};

Preprocessor::~Preprocessor()
{
  delete lemmatizer;
}

bool Preprocessor::isLetter(int c)
{
  // If c<0 is not ascii. Could be a latin character
  return (c<0) || ('a'<=c && c<='z') || ('A'<=c && c<='Z');
}

void Preprocessor::loadLemmatizerDic(std::string lemmadic_filename)
{
  // Check if file exists
  std::ifstream infile(lemmadic_filename);
  if (!infile.good()){
    std::cout << "Can not open lemmagen dictionary";
    throw "Can not open lemmagen dictionary";
  }

  // Load library
  const char *cstr_filename = lemmadic_filename.c_str();
  this->lemmatizer = new RdrLemmatizer(cstr_filename);
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

  const char* cstr_word = word.c_str();

  // const char *cstr_word = word.c_str();
  char* lemmatized_word = this->lemmatizer->Lemmatize(cstr_word);
  std::cout << lemmatized_word << std::endl;
  word = std::string(lemmatized_word);
  delete[] lemmatized_word;
}

void Preprocessor::preprocess() {
  // Open files
  std::ifstream infile(this->infilename);
  std::ofstream outfile(this->outfilename);

  if (!infile.is_open() || !outfile.is_open())
    throw "Can not open input or output file";

  // Read infile
  std::string line;
  while(getline(infile, line))
  {
    // Separate words in line
    std::stringstream line_stream(line);
    std::string word;
    while (getline(line_stream, word, ' '))
    {
      this->tokenize(word);
      this->lowerStr(word);
      this->removeStopword(word);
      this->lemmatize(word);

      if (!word.empty())
        outfile << word << "\n";
    }
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