#include "SearchEngineManager.h"

int main(int argc, char* argv[])
{
    SearchEngineManager sem("data/zim/wiki-mini.zim", "../website/btree.index", "../website/wordpage.index", "../website/keyword.index");
    sem.process ();
    return 0;
}