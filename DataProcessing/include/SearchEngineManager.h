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

    void add_word (bptree &, diskManager &, const str, const int, const int);

public:
    SearchEngineManager(const str ="", const str ="", const str ="");
    ~SearchEngineManager();

    void process ();
    void print_search_word (const str);
    void print_search_text (const str);
    std::vector<size_t> search(const str);
    void search_text_parallel (const str, int);

};

#endif