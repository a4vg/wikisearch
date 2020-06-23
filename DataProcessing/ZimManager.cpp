#include "ZimManager.h"

ZimManager::ZimManager(const std::string& filename) {
    auto f = new zim::File(filename);
    file = *f;
}

ZimManager::iterator ZimManager::begin() {
    iterator iter;
    iter.i = file.begin();
    while(iter.i->getLibraryMimeType() != ACCEPTED_MYMETYPE)
        iter.i++;
    iter.update_values();
    return iter;
}

ZimManager::iterator ZimManager::end() {
    iterator iter;
    iter.i = file.end();
    // iter.update_values();
    return iter;
}

ZimManager::iterator ZimManager::iterator::operator++() {
    i++;
    while (i->getLibraryMimeType() != ACCEPTED_MYMETYPE)
        i++;
    update_values();
    return *this;
}

bool ZimManager::iterator::operator!=(const ZimManager::iterator& it2) {
    return i != it2.i;
}

void ZimManager::iterator::update_values() {
    value.first = i.getIndex();
    value.second = i->getRedirectArticle().getData();
}

std::pair<int, std::string> ZimManager::iterator::operator*() {
    return this->value;
}

ZimManager::iterator::iterator() {
    value.first = 0;
    value.second = "";
}