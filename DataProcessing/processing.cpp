#include <iostream>
#include "SearchEngineManager.h"

int main(int argc, char* argv[])
{
    SearchEngineManager sem;
    sem.process ();
    sem.search_text ("unir wisconsin seguir de Global juego");

    return 0;
}