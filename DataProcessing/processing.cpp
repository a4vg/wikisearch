#include "ZimManager.h"
#include "Preprocessor.h"
#include <iostream>

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

    for(auto iter = manager.begin(); iter!=manager.end(); ++iter)
    {
        std::cout << "\n=====================================================\n";
        std::cout << "Article #" << (*iter).first << "\n";
        preprocessor.setText((*iter).second);
        auto wordCount = preprocessor.getWordCount();
        for (const auto& count: wordCount)
            std::cout << count.first << ": " << count.second << "\n";
    }

    return 0;
}