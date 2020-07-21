#include <iostream>
#include "include/SearchEngineManager.h"

int main(int argc, char* argv[])
{
    SearchEngineManager sem;
    // sem.process ();
    // sem.print_search_text ("unir wisconsin seguir de Global juego");

    auto file = zim::File ("../DataProcessing/data/zim/wiki-mini.zim");

    for(auto&nsq:sem.ranked_search("union peru"))
        std::cout<<nsq<<" -> " << file.getArticle(nsq).getTitle() <<'\n';

    return 0;
}