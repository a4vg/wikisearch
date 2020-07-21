#ifndef SEARCHENGINE
#define SEARCHENGINE

#include "include/SearchEngineManager.h"

bool compare(std::pair<size_t, double> a, std::pair<size_t, double> b){
    return a.second > b.second;
}

SearchEngineManager::SearchEngineManager(const str zfile, const str tfile,
    const str wfile): zimfile (zfile), treefile (tfile), wordsfile (wfile)
{
    if (zimfile.empty())
        zimfile = "../DataProcessing/data/zim/wiki-mini.zim";
    if (treefile.empty())
        treefile = "../DataProcessing/btree.index";
    if (wordsfile.empty())
        wordsfile = "../DataProcessing/wordpage.index";

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

    int total = 0;
    pm2->write_record (0, total);

    bptree mytree (pm1);

    for(auto iter = manager->begin(); iter!=manager->end(); ++iter)
    {
        preprocessor.setText((*iter).second);
        auto wordCount = preprocessor.getWordCount();
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

    bptree mtree (pm1);
    Cadena cad ((char *) word.c_str());
    auto file = zim::File (zimfile);

    int id = mtree.search (cad);

    if (id != -1) {
        std::cout << word << " was found\n";
        std::pair<int, int> results [MAX_ARTICLES];
        pm2->retrieve_record (id, results);
        for (int i = 0; i < MAX_ARTICLES; i++){
            if (results[i].first > 0)
            std::cout << "Article " << results[i].first <<
            ": - Count:" << results[i].second << ", Title: "
            << file.getArticle(results[i].first).getTitle() << ", Namespace: "
            << file.getArticle(results[i].first).getNamespace() << "\n";
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
    auto file = zim::File (zimfile);

    for (const auto& count: wordstext)
        print_search_word (count.first);
}

std::vector<size_t> SearchEngineManager::ranked_search(const str text){
    std::set<size_t> results = search(text);
    preprocessor.setText(text);
    std::map<size_t, double> m_rank;
    double tf_idf;

    for(auto&term:preprocessor.getWordCount()){
        for(auto&document:results){
            tf_idf = tfidf(document, term.first, results);
            m_rank[document] += tf_idf;
            std::cout << term.first << 'x' << document << ':' << m_rank[document] << '\n';
        }
    }

    std::vector<std::pair<size_t, double>> rank;
    for(auto&p:m_rank)
        rank.push_back(p);

    std::sort(rank.begin(), rank.end(), compare);
    std::vector<size_t> top_results(MAX_ARTICLES);

    for(int i=0;i<MAX_ARTICLES;++i)
        top_results[i] = rank[i].first;

    return top_results;
}


std::set<size_t> SearchEngineManager::search(const str text)
{
    std::set<size_t> matches;

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
        for (auto & result : results){
            if (result.first > 0)
                matches.insert(result.first);
        }
    }
    
    return matches;
}

void SearchEngineManager::search_text_parallel (const str text, int numthreads)
{
}

double SearchEngineManager::tfidf(size_t document, str term, std::set<size_t> results) {

    int df = 0;
    std::map<std::string, int> wordCount;
    std::map<std::string, int>::iterator it;
    for(auto&doc:results){
        preprocessor.setText((*manager->getIteratorFromArticleId(doc)).second);
        wordCount = preprocessor.getWordCount();
        it = wordCount.find(term);
        if(it != wordCount.end())
            df++;
    }

    preprocessor.setText((*manager->getIteratorFromArticleId(document)).second);
    wordCount = preprocessor.getWordCount();

    it = wordCount.find(term);
    double tf = 1;
    if(it != wordCount.end())
        tf += it->second / (double) wordCount.size();

    double idf = log((double)results.size()/(df+1));

    return tf / idf;
}

#endif