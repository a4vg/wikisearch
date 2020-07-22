#ifndef SEARCHENGINEMANAGERH
#define SEARCHENGINEMANAGERH

#include "ZimManager.h"
#include "Preprocessor.h"
#include "disk_manager.hpp"
#include "Tree.hpp"
#include <iostream>
#include <memory>
#include "string.h"
#include <vector>
#include <sstream>
#include <algorithm> // partial_sort
#include <utility> // pair
#include <thread>
#include <mutex>
#include <set>

#define MAX_KEYWORDS 3
#define MAX_ARTICLES 10
#define ORDER 4



class SearchEngineManager
{
private:
    typedef typename std::shared_ptr<DiskManager> diskManager;
    typedef BPNode<Cadena, ORDER> bpnode;
    typedef Tree<bpnode, true> bptree;
    typedef std::string str;
    /* data */

    ZimManager * manager;
    str zimfile;
    Preprocessor preprocessor;
    str treefile;
    str wordsfile;
    str keywordfile;

    std::vector <std::thread *> threads;
    std::mutex mtx;

    void add_word (bptree &, diskManager &, const str, const int, const int);

public:
    SearchEngineManager(const str ="", const str ="", const str ="", const str ="");
    ~SearchEngineManager();

    std::vector<str> getArticleKeywords(size_t idx);
    void process (ZimManager::iterator itbegin, bool print=false, int print_count=100);
    void print_search_word (const str);
    void print_search_text (const str);
    std::set<size_t> search(const str);
    std::vector<size_t> ranked_search(const str);
    std::vector<size_t> search_text_parallel (const str);
    void subsearch (const std::vector<str> , std::vector <size_t>&);
    double tfidf(size_t, str, std::set<size_t>);
};

#endif