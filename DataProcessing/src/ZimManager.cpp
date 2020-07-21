#ifndef ZIMMANAGER_CPP
#define ZIMMANAGER_CPP

#include "ZimManager.h"

bool isAcceptedMIMEtype(int m)
{
    return m==ACCEPTED_MYMETYPE1 || m==ACCEPTED_MYMETYPE2;
}

ZimManager::ZimManager(const std::string& filename) {
    auto f = new zim::File(filename);
    file = *f;
}

ZimManager::iterator ZimManager::begin() {
    iterator iter(*this);
    iter.i = file.begin();
    while(!this->isValidArticle(iter.i->getIndex()))
        iter.i++;
    iter.update_values();
    return iter;
}

ZimManager::iterator ZimManager::end() {
    iterator iter(*this);
    iter.i = file.end();
    // iter.update_values();
    return iter;
}

ZimManager::iterator ZimManager::iterator::operator++() {
    i++;
    while (i!=file.end() && !ZimManager::isValidArticle(*i))
        i++;

    if (i!=file.end())
      update_values();
    return *this;
}

ZimManager::iterator ZimManager::getIteratorFromArticleId(size_t idx)
{
    iterator iter(*this);
    iter.i = zim::File::const_iterator(&this->file, idx);
    iter.update_values();
    return iter;
}

std::string ZimManager::getArticleHtml(size_t idx)
{
    auto a = file.getArticle(idx);
    if (a.isRedirect())
        a = a.getRedirectArticle();
    zim::Blob blob = a.getData();
    return std::string(blob.data(), blob.size());
}

bool ZimManager::isValidArticle(size_t idx)
{
    return ZimManager::isValidArticle(file.getArticle(idx));
}

bool ZimManager::isValidArticle(const zim::Article &a)
{
    return a.getNamespace() == 'A' &&
           !a.isRedirect() &&
           isAcceptedMIMEtype(a.getLibraryMimeType());
}

std::string ZimManager::getArticleTitle(size_t idx)
{
    return std::string(file.getArticle(idx).getTitle());
}

bool ZimManager::iterator::operator!=(const ZimManager::iterator& it2) {
    return i != it2.i;
}

void ZimManager::iterator::update_values() {
    value.first = i.getIndex();
    zim::Blob blob = i->getData();
    value.second = parseWikipediaHTML(blob.data(), blob.size());
}

std::pair<int, std::string> ZimManager::iterator::operator*() {
    return this->value;
}

ZimManagerIterator::ZimManagerIterator(ZimManager& z) {
    value.first = 0;
    value.second = "";
    file = z.file;
}

#endif