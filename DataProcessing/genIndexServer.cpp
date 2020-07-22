#include "SearchEngineManager.h"

int main(int argc, char* argv[])
{
    SearchEngineManager sem("data/zim/wiki-mini.zim", "../website/btree.index", "../website/wordpage.index", "../website/keyword.index");
    ZimManager zm ("data/zim/wiki-mini.zim");
    sem.process (zm.begin());
    return 0;
}