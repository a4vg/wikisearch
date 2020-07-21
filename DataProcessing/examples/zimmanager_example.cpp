//  g++ --std=c++11 -I../include -lmyhtml -lzim zimmanager_example.cpp ../src/htmlParser.cpp ../src/ZimManager.cpp

#include "ZimManager.h"

int main(int argc, char* argv[])
{
    if (argc<2)
    {
        std::cerr << "You need to provide a zim file" << std::endl;
        exit(1);
    }

    const char* filename = argv[1];
    try {
        ZimManager manager(argv[1]);

        for(auto iter = manager.begin(); iter!=manager.end(); ++iter) {
            const size_t idx = (*iter).first;
            std::cout << "[ " << idx << "] "; // << (*iter).second << '\n';
            std::cout << manager.getArticleTitle(idx) <<"\n";
            // std::cout << manager.getArticleHtml(idx) <<"\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

