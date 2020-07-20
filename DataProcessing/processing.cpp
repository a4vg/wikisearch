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

const int MAX_ARTICLES = 10;

void add_word (bptree &bt, diskManager &dm, const std::string word, const int count, const int articleId) {
    int total = 0;
    dm->retrieve_record (0, total);
    Cadena cadena ((char *) word.c_str());
    int pid = bt.search (cadena);
    if (pid == -1) {
        int newpageId = total + 1;
        bt.insert (cadena, newpageId);
        std::pair<int, int> articles [MAX_ARTICLES];
        for (int i = 0; i < MAX_ARTICLES; i++) {
            articles [i] = std::make_pair (0, 0);
        }
        articles [0] = std::make_pair (articleId, count);
        dm->write_record (newpageId, articles);
        total ++;
        dm->write_record (0, total);
    } else {
        std::pair<int, int> articles [MAX_ARTICLES];
        dm->retrieve_record (pid, articles);
        int i = 0;
        auto temp = std::make_pair (articleId, count);
        for (; i < MAX_ARTICLES; i++) {
            if (articles [i].second != 0){
                if (articles [i].second < temp.second){
                    auto t = articles [i];
                    articles [i] = temp;
                    temp = t;
                }
            } else {
                articles [i] = temp;
            }
        }
        dm->write_record (pid, articles);
    }
}

int main(int argc, char* argv[])
{
    if (argc<2)
    {
        std::cerr << "You need to provide a zim file" << std::endl;
        exit(1);
    }

    const char* filename = argv[1];
    Preprocessor preprocessor;

    ZimManager manager(argv[1]);

    int total = 0;

    std::shared_ptr<DiskManager> pm1 = std::make_shared<DiskManager> ("btree.index", true);
    std::shared_ptr<DiskManager> pm2 = std::make_shared<DiskManager> ("wordspage.index", true);
    pm2->write_record (0, total);
    bptree mytree (pm1);

    for(auto iter = manager.begin(); iter!=manager.end(); ++iter)
    {
        std::cout << "\n=====================================================\n";
        std::cout << "Article #" << (*iter).first << "\n";
        preprocessor.setText((*iter).second);
        auto wordCount = preprocessor.getWordCount();
        for (const auto& count: wordCount) {
            std::cout << count.first << ": " << count.second << "\n";
            add_word (mytree, pm2, count.first, count.second, (*iter).first);
        }
    }

    return 0;
}