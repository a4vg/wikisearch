#include <zim/file.h>
#include <zim/fileiterator.h>
#include <iostream>
#include <set>
#include <fstream>
using namespace std;
#define N 1
#include "htmlParser.h"

/*
LibraryMymeType

0  applicatio/javascript
3  image/gif
4  image/png
5  image/svg+xml
6  text/css
7  text/html  (pero está raro)
65535 html (retorna un html)
*/


int main(int argc, char* argv[])
{

    try
    {
        ofstream f("articulo.html");

        //To control the number of articles
        int c = 0;

        //The zim file
        zim::File file("wikipedia.zim");
        for (zim::File::const_iterator it = file.begin(); it != file.end(); ++it)
        {
            if (c == N){
                break;
            }
            try {
                int MymeType = it->getLibraryMimeType();
                // Filtrando los documentos html
                if (MymeType ==65535) {

                    zim::Article article = it->getRedirectArticle();
                    zim::Blob blob = article.getData();
                    std::string stringdata = std::string(blob.data(), blob.size());
                    const char *zptr = stringdata.c_str();  // c_str() guarantees, that the pointer points to zero terminated data
                    std::cout << zptr << std::endl;
                    string h(zptr);
                    cout << parse_html(h);

                    c ++ ;
                    f << zptr;
                    f.close();

                }
            } catch (exception exception) {
                cerr << exception.what() << endl;
            }
        }
    }

    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}