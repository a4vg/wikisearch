#include <iostream>
#include "SearchEngineManager.h"
#include "ZimManager.h"

int main(int argc, char* argv[])
{
    ZimManager zimmanager("./data/zim/wiki-mini.zim");
    SearchEngineManager sem("./data/zim/wiki-mini.zim", "../website/btree.index", "../website/wordpage.index");
    sem.process();
    // sem.print_search_text ("unir wisconsin seguir de Global juego");
    // sem.print_search_text("unir wisconsin seguir de Global juego");
    // for (size_t id: matchesId)
    // {

    //     auto it = zimmanager.getIteratorFromArticleId(id);
    //     std::cout << "ID: " << (*it).first << "\n";
    //     // std::cout << "id: " << id << "\n";
    //     // std::cout << (*it).second << "\n\n";
    // }

    return 0;
}