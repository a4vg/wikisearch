#include <iostream>
#include "SearchEngineManager.h"

int main(int argc, char* argv[])
{
    SearchEngineManager sem;
    sem.process ();
    sem.print_search_text ("unir wisconsin seguir de Global juego");
    sem.print_search_text ("unir wisconsin seguir encontrar");
    sem.print_search_text ("unir wisconsin seguir edad encontrar andar america ultimo");
    std::cout << "\nParalel 1:\n";
    for (auto &res: sem.search_text_parallel ("unir wisconsin seguir"))
        std::cout << "Article " << res << ", ";
    std::cout << "\n\nParalel 2:\n";
    for (auto &res: sem.search_text_parallel ("unir wisconsin seguir encontrar"))
        std::cout << "Article " << res << ", ";
    std::cout << "\n\nParalel 3:\n";
    for (auto &res: sem.search_text_parallel ("unir wisconsin seguir edad encontrar andar america ultimo"))
        std::cout << "Article " << res << ", ";

    std::cout << "\n\n=====================================================\n";

    return 0;
}