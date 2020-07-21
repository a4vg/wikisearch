#ifndef ZIMMANAGER_CPP
#define ZIMMANAGER_CPP

#include "ZimManager.h"

ZimManager::ZimManager(const std::string& filename) {
    auto f = new zim::File(filename);
    file = *f;
}

ZimManager::iterator ZimManager::begin() {
    iterator iter(*this);
    iter.i = file.begin();
    while(iter.i->getLibraryMimeType() != ACCEPTED_MYMETYPE)
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
    while (i!=file.end() && i->getLibraryMimeType() != ACCEPTED_MYMETYPE)
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
    zim::Blob blob = file.getArticle(idx).getRedirectArticle().getData();
    return std::string(blob.data(), blob.size());
}

bool ZimManager::isValidArticle(size_t idx)
{
    return file.getArticle(idx).getNamespace() == 'A' &&
           file.getArticle(idx).getLibraryMimeType() == ACCEPTED_MYMETYPE;
}

std::string ZimManager::getArticleTitle(size_t idx)
{
    auto a = file.getArticle(idx);

    std::cout << "Is redirect?: " << a.isRedirect() << "\n";
    std::cout << "Redirected article: " << a.getRedirectArticle().getIndex() 
              << "\nRedirected mimetype: " << a.getLibraryMimeType() << "\n";
    return std::string(a.getTitle()) + " **** " + std::string(a.getRedirectArticle().getTitle());
}

bool ZimManager::iterator::operator!=(const ZimManager::iterator& it2) {
    return i != it2.i;
}

void ZimManager::iterator::update_values() {
    value.first = i.getIndex();
    zim::Blob blob = i->getRedirectArticle().getData();
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