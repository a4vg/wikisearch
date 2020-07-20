//
// Created by notpyxl on 23/06/20.
//

#ifndef CS3102_TRABAJOFINAL_COVID_20_ZIMMANAGER_H
#define CS3102_TRABAJOFINAL_COVID_20_ZIMMANAGER_H

#include <zim/file.h>
#include <zim/fileiterator.h>
#include "htmlParser.h"

#define ACCEPTED_MYMETYPE 65535

class ZimManager;

class ZimManagerIterator{
    std::pair<int, std::string> value;
    zim::File file;

public:
    ZimManagerIterator(ZimManager& z);
    zim::File::const_iterator i;
    ZimManagerIterator operator ++();
    bool operator !=(const ZimManagerIterator& it2);
    std::pair<int,std::string> operator *();
    void update_values();
};

class ZimManager {

public:
    typedef ZimManagerIterator iterator;

    zim::File file;
    explicit ZimManager(const std::string&);

    iterator getIteratorFromArticleId(size_t idx);
    iterator begin();
    iterator end();
};


#endif //CS3102_TRABAJOFINAL_COVID_20_ZIMMANAGER_H
