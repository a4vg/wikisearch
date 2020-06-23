#include "ZimManager.h"

int main(int argc, char* argv[])
{
    const char* filename = argv[1];
    try {
        ZimManager manager(argv[1]);

        for(auto iter = manager.begin(); iter!=manager.end(); ++iter) {
            std::cout << (*iter).first << ' ' << (*iter).second << '\n';
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

