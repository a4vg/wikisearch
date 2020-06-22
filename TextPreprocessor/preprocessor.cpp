#include <iostream>
#include <string>
#include <vector>
#include <fstream> // ifstream, ofstream
#include <sstream> // stringstream
#include <cctype> // tolower

class Preprocessor {
  std::string infilename;
  std::string outfilename;
  std::vector<std::string> stopwords;

  bool isLetter(int c);
  void loadStopwords(std::string stopwords_filename);
  void removeStopword(std::string &word);
  void lowerStr(std::string &word);
  void tokenize(std::string &word);

  public:
    Preprocessor(std::string _infilename, std::string _outfilename, std::string sw_filename="stopwords-es.txt");
    void preprocess();
};

Preprocessor::Preprocessor
(std::string _infilename, std::string _outfilename, std::string sw_filename):
infilename(_infilename), outfilename(_outfilename)
{
  loadStopwords(sw_filename);
};

bool Preprocessor::isLetter(int c)
{
  // If c<0 is not ascii. Could be a latin character
  return (c<0) || ('a'<=c && c<='z') || ('A'<=c && c<='Z');
}

void Preprocessor::loadStopwords(std::string sw_filename)
{
  std::ifstream sw_file(sw_filename);
  if (!sw_file.is_open()){
    throw "Can not open stopword file";
  }

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
  Preprocessor preprocessor("text-example.txt", "output.txt", "stopwords-es.txt");
  preprocessor.preprocess();
}