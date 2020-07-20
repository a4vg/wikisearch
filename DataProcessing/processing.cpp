#include <iostream>
#include "SearchEngineManager.h"

int main(int argc, char* argv[])
{
    SearchEngineManager sem;
    sem.process ();
    sem.print_search_text ("unir wisconsin seguir de Global juego");

    return 0;
}