// g++ --std=c++11 -I../include -lmyhtml ../src/htmlParser.cpp htmlParser_example.cpp

#include <iostream>
#include <fstream>
#include <sstream>

#include "htmlParser.h"

int main()
{
    std::ifstream file("htmlParser_example.html");
    std::stringstream strStream;
    strStream << file.rdbuf(); //read the file
    std::string html_s = strStream.str();

    char const* html = html_s.c_str();

    std::cout << parseWikipediaHTML(html, html_s.length());
}