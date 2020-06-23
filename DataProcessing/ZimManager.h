//
// Created by notpyxl on 23/06/20.
//

#ifndef CS3102_TRABAJOFINAL_COVID_20_ZIMMANAGER_H
#define CS3102_TRABAJOFINAL_COVID_20_ZIMMANAGER_H

#include <zim/file.h>
#include <zim/fileiterator.h>
#include <zim/fileimpl.h>

#define ACCEPTED_MYMETYPE 65535

class ZimManager {

public:

    zim::File file;
    explicit ZimManager(const std::string&);

    struct iterator{
        iterator();

        zim::File::const_iterator i;
        iterator operator ++();
        bool operator !=(const iterator& it2);
        std::pair<int,std::string> operator *();
        void update_values();

    private:
        std::pair<int, std::string> value;
    };

    iterator begin();
    iterator end();
};


#endif //CS3102_TRABAJOFINAL_COVID_20_ZIMMANAGER_H
