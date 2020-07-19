#include "Trie.h"
#include "Preprocessor.h"
#include <iostream>
#include <fstream>
#include <string>

int main()
{
    Trie<bool> t;
    std::ifstream file("./data/lemma-dictionaries/lemmatization-es.txt");
    if (!file.is_open())
        throw "Can not open file";

    std::string word, trash;
    while (file >> trash >> word)
    {
      Preprocessor::removeTilde(word);
      t.insert(word, true);
    }
    t.exportAsJson("trie.json");
}