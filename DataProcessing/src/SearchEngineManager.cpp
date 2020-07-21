#ifndef SEARCHENGINE
#define SEARCHENGINE

#include "SearchEngineManager.h"


SearchEngineManager::SearchEngineManager(const str zfile, const str tfile,
    const str wfile, const str kwfile)
    : zimfile (zfile), treefile (tfile), wordsfile (wfile), keywordfile(kwfile)
{
    if (zimfile.empty())
        zimfile = "data/zim/wiki-mini.zim";
    if (treefile.empty())
        treefile = "btree.index";
    if (wordsfile.empty())
        wordsfile = "wordpage.index";
    if (keywordfile.empty())
        keywordfile = "keyword.index";

    manager = new ZimManager (zimfile);
}

SearchEngineManager::~SearchEngineManager()
{
}

void SearchEngineManager::add_word (bptree &bt, diskManager &dm, const str word,
                                const int count, const int articleId)
{
    int total = 0;
    dm->retrieve_record (0, total);
    Cadena cadena ((char *) word.c_str());
    int pid = bt.search (cadena);
    if (pid == -1) {
        int newpageId = total + 1;
        bt.insert (cadena, newpageId);
        std::pair<int, int> articles [MAX_ARTICLES];
        for (int i = 0; i < MAX_ARTICLES; i++) {
            articles [i] = std::make_pair (0, 0);
        }
        articles [0] = std::make_pair (articleId, count);
        dm->write_record (newpageId, articles);
        total ++;
        dm->write_record (0, total);
    } else {
        std::pair<int, int> articles [MAX_ARTICLES];
        dm->retrieve_record (pid, articles);
        int i = 0;
        auto temp = std::make_pair (articleId, count);
        for (; i < MAX_ARTICLES; i++) {
            if (articles [i].second != 0){
                if (articles [i].second < temp.second){
                    auto t = articles [i];
                    articles [i] = temp;
                    temp = t;
                }
            } else {
                articles [i] = temp;
                break;
            }
        }
        dm->write_record (pid, articles);
    }
}

void SearchEngineManager::process ()
{
    std::shared_ptr<DiskManager> pm1 = 
        std::make_shared<DiskManager> (treefile, true);
    std::shared_ptr<DiskManager> pm2 = 
        std::make_shared<DiskManager> (wordsfile, true);
    std::shared_ptr<DiskManager> kwpm = 
        std::make_shared<DiskManager> (keywordfile, true);

    int total = 0;
    pm2->write_record (0, total);
    kwpm->write_record (0, total);

    bptree mytree (pm1);

    for(auto iter = manager->begin(); iter!=manager->end(); ++iter)
    {
        preprocessor.setText((*iter).second);
        auto wordCount = preprocessor.getWordCount();

        // // Get the MAX_KEYWORDS most frequent words in article
        std::vector<std::pair<str, int> > wordsFreq;
        str keywords[MAX_KEYWORDS]; // empty strings
        for (auto& count: wordCount) {
            wordsFreq.push_back(count);
        }

        // Sort only the MAX_KEYWORDS
        const int max_kw = MAX_KEYWORDS>wordCount.size()? wordCount.size() : MAX_KEYWORDS;
        std::partial_sort(wordsFreq.begin(), wordsFreq.begin() + max_kw, wordsFreq.end(),
        [](const std::pair<str,int> &l, std::pair<str,int> &r)
        {
            return l.second > r.second; // sort in descending order by count
        });

        // Get the keywords and store them
        for (int i=0; i<max_kw; ++i)
        {
            keywords[i] = wordsFreq[i].first; // don't store count, only words
        }

        kwpm->write_record (0, ++total);
        kwpm->write_record ((*iter).first, keywords);
        
        for (const auto& count: wordCount) {
            add_word (mytree, pm2, count.first, count.second, (*iter).first);
        }
    }
}

void SearchEngineManager::print_search_word (const str word)
{
    if (word.empty ()){
        std::cerr << "You need to provide a word to search" << std::endl;
        exit(1);
    }

    std::shared_ptr<DiskManager> pm1 = 
        std::make_shared<DiskManager> (treefile, false);
    std::shared_ptr<DiskManager> pm2 = 
        std::make_shared<DiskManager> (wordsfile, false);
    std::shared_ptr<DiskManager> kwpm = 
        std::make_shared<DiskManager> (keywordfile, false);

    bptree mtree (pm1);
    Cadena cad ((char *) word.c_str());

    int id = mtree.search (cad);

    if (id != -1) {
        std::cout << word << " was found\n";
        std::pair<int, int> results [MAX_ARTICLES];
        pm2->retrieve_record (id, results);

        for (int i = 0; i < MAX_ARTICLES; i++){
            
            const size_t artId = results[i].first;
            
            if (artId > 0)
            {
                str keywords[MAX_KEYWORDS];
                kwpm->retrieve_record (artId, keywords);

                std::cout << "Article " << artId << 
                ": - Count:" << results[i].second << ", Title: " 
                <<  manager->getArticleTitle(artId) << ", Valid article?: "
                << manager->isValidArticle(artId) << "\n"
                << "Keywords: ";
                for (size_t kwi=0; kwi<MAX_KEYWORDS; ++kwi)
                    std::cout << keywords[kwi] << " ";
                std::cout << "\n";
            }
            
        }
    } else {
        std::cout << "Not found " << word << "\n";
    }
}

void SearchEngineManager::print_search_text (const str text)
{
    if (text.empty ()){
        std::cerr << "You need to provide a word to search" << std::endl;
        exit(1);
    }

    preprocessor.setText (text);
    auto wordstext = preprocessor.getWordCount ();

    for (const auto& count: wordstext) 
        print_search_word (count.first);
}


std::vector<size_t> SearchEngineManager::search(const str text)
{
    std::vector<size_t> matches;

    std::shared_ptr<DiskManager> pm_tree = 
        std::make_shared<DiskManager> (treefile, false);
    std::shared_ptr<DiskManager> pm_descriptor = 
        std::make_shared<DiskManager> (wordsfile, false);

    bptree tree (pm_tree);
    preprocessor.setText(text);
    auto wordstext = preprocessor.getWordCount();
    for (const auto& count: wordstext)
    {
        Cadena cad((char *) count.first.c_str());
        int id = tree.search(cad);
        if (id == -1)
            continue;
        std::pair<int, int> results [MAX_ARTICLES];
        pm_descriptor->retrieve_record (id, results);
        for (int i = 0; i < MAX_ARTICLES; i++){
            if (results[i].first > 0)
                matches.push_back(results[i].first);
        }
    }
    
    return matches;
}

void SearchEngineManager::search_text_parallel (const str text, int numthreads)
{
}

#endif