// g++ --std=c++11 -I./include -I./lib searchword.cpp -lzim

#include "ZimManager.h"
#include "Preprocessor.h"
#include "disk_manager.hpp"
#include "Tree.hpp"
#include <iostream>
#include <memory>
#include "string.h"
#include <vector>

typedef typename std::shared_ptr<DiskManager> diskManager;
typedef BPNode<Cadena, 4> bpnode;
typedef Tree<bpnode, true> bptree;

int main(int argc, char* argv[])
{
    if (argc<2)
    {
        std::cerr << "You need to provide a word to search" << std::endl;
        exit(1);
    }

    const char* filename = "data/zim/wiki-mini.zim";

    const char* query = argv[1];
    std::shared_ptr<DiskManager> pm1 = std::make_shared<DiskManager> ("btree.index", false);
    std::shared_ptr<DiskManager> pm2 = std::make_shared<DiskManager> ("wordspage.index", false);

    bptree mtree (pm1);
    Cadena cad ((char *)query);

    auto file = zim::File (filename);

    int id = mtree.search (cad);
    if (id != -1) {
        std::cout << "Found\n";
        std::pair<int, int> results [10];
        pm2->retrieve_record (id, results);
        for (int i = 0; i < 10; i++){
            if (results[i].first > 0)
            std::cout << "Article " << results[i].first << ": - Count:" << results[i].second << ", Title: " 
            <<  file.getArticle(results[i].first).getTitle() << ", Namespace: " << file.getArticle(results[i].first).getNamespace() << "\nURL: "
            <<  file.getArticle(results[i].first).getUrl() << "\n";
        }
    } else {
        std::cout << "Not found\n";
    }
   

}

//unir wisconsin seguir retener global