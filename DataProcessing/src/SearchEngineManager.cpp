#ifndef SEARCHENGINE
#define SEARCHENGINE

#include "SearchEngineManager.h"


SearchEngineManager::SearchEngineManager(const str zfile, const str tfile,
    const str wfile): zimfile (zfile), treefile (tfile), wordsfile (wfile)
{
    if (zimfile.empty())
        zimfile = "data/zim/wiki-mini.zim";
    if (treefile.empty())
        treefile = "btree.index";
    if (wordsfile.empty())
        wordsfile = "wordpage.index";

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
    diskManager pm1 = 
        std::make_shared<DiskManager> (treefile, true);
    diskManager pm2 = 
        std::make_shared<DiskManager> (wordsfile, true);

    int total = 0;
    pm2->write_record (0, total);

    bptree mytree (pm1);

    for(auto iter = manager->begin(); iter!=manager->end(); ++iter)
    {
        std::cout << "\n=====================================================\n";
        std::cout << "Article #" << (*iter).first << "\n";
        preprocessor.setText((*iter).second);
        auto wordCount = preprocessor.getWordCount();
        for (const auto& count: wordCount) {
            std::cout << count.first << ": " << count.second << "\n";
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

    diskManager pm1 = 
        std::make_shared<DiskManager> (treefile, false);
    diskManager pm2 = 
        std::make_shared<DiskManager> (wordsfile, false);

    bptree mtree (pm1);
    Cadena cad ((char *) word.c_str());

    int id = mtree.search (cad);

    if (id != -1) {
        std::cout << word << " was found\n";
        std::pair<int, int> results [MAX_ARTICLES];
        pm2->retrieve_record (id, results);
        for (int i = 0; i < MAX_ARTICLES; i++){
            const size_t idx = results[i].first;
            if (results[i].first > 0)
            std::cout << "Article " << idx << 
            ": - Count:" << results[i].second << ", Title: " 
            <<  manager->getArticleTitle(idx) << ", Valid article?: "
            << manager->isValidArticle(idx) << "\n";
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


void SearchEngineManager::subsearch (const std::vector<str> vec, std::vector <size_t> &matches) {
    diskManager pm_tree = 
        std::make_shared<DiskManager> (treefile, false);
    diskManager pm_descriptor = 
        std::make_shared<DiskManager> (wordsfile, false);

    bptree tree (pm_tree);

    for (auto& c: vec) {
        Cadena cad((char *) c.c_str());
        int id = tree.search (cad);
        if (id == -1)
            return;
        std::pair<int, int> results [MAX_ARTICLES];
        pm_descriptor->retrieve_record (id, results);
        for (int i = 0; i < MAX_ARTICLES; i++){
            if (results[i].first > 0){
                mtx.lock ();
                matches.push_back(results[i].first);
                mtx.unlock ();
            }
        }
    }
}

std::vector<size_t> SearchEngineManager::search(const str text)
{
    std::vector<size_t> matches;

    diskManager pm_tree = 
        std::make_shared<DiskManager> (treefile, false);
    diskManager pm_descriptor = 
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

std::vector<size_t> SearchEngineManager::search_text_parallel (const str text)
{
    threads.clear();
    unsigned int max_threads = std::thread::hardware_concurrency ();
    //std::cout << "Max: " << max_threads << "\n";
    std::vector<size_t> matches;

    preprocessor.setText(text);
    auto wordstext = preprocessor.getWordCount();
    
    int wordsperthread = wordstext.size() / max_threads;

    std::vector <str> wordsforthread;
    if (max_threads < wordstext.size ()) {
        int total = 0;
        int number_ofthread = 0;
        for (const auto& count: wordstext) {
            wordsforthread.push_back (count.first);
            total ++;
            if (total >= wordsperthread && number_ofthread < max_threads - 1) {
                threads.push_back( new std::thread (&SearchEngineManager::subsearch, this, wordsforthread, std::ref(matches)));
                wordsforthread.clear ();
                number_ofthread++;
                total = 0;
            }
        }
        threads.push_back (new std::thread (&SearchEngineManager::subsearch, this, wordsforthread, std::ref(matches)));
    } else {
        for (const auto& count: wordstext) {
            wordsforthread.clear ();
            wordsforthread.push_back (count.first);
            threads.push_back(new std::thread (&SearchEngineManager::subsearch, this, wordsforthread, std::ref(matches)));
        }
    }

    for (int i = 0; i < threads.size(); i++) {
        threads[i]->join();
        delete threads [i];
    }

    return matches;

    /*for (auto& m: matches)
        std::cout << m << " ";*/
    //std::cout << "\n" << matches.size() << "\n";
}

#endif