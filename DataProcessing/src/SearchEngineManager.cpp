#ifndef SEARCHENGINE
#define SEARCHENGINE

#include "SearchEngineManager.h"

bool compare(std::pair<size_t, double> a, std::pair<size_t, double> b){
    return a.second < b.second;
}

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
    //int pid = bt.search (cadena);
    int pid = bt.insert (cadena, total + 1);
    if (pid == -1) {
        int newpageId = total + 1;
        //bt.insert (cadena, newpageId);
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

void SearchEngineManager::process (ZimManager::iterator itbegin, bool print, int print_count)
{
    bool trunc = ((*itbegin).first == (*manager->begin()).first)? true : false;

    diskManager pm1 = 
        std::make_shared<DiskManager> (treefile, trunc);
    diskManager pm2 = 
        std::make_shared<DiskManager> (wordsfile, trunc);
    std::shared_ptr<DiskManager> kwpm = 
        std::make_shared<DiskManager> (keywordfile, trunc);

    int total = 0;
    pm2->write_record (0, total);
    kwpm->write_record (0, total);

    bptree mytree (pm1);
    size_t totalArticles = manager->getTotalArticles();

    int c = 0;

    for(auto iter = itbegin; iter!=manager->end(); ++iter)
    {
        if (c%print_count==0 && print)
            std::cout << c << " valid articles processed. "
            << "Current ID: "<< (*iter).first
            << " from a total of : "<< totalArticles << "\n";
        ++c;

        preprocessor.setText((*iter).second);
        auto wordCount = preprocessor.getWordCount();

        // // Get the MAX_KEYWORDS most frequent words in article
        std::vector<std::pair<str, int> > wordsFreq;
        //str keywords[MAX_KEYWORDS]; // empty strings
        Cadena keywords [MAX_KEYWORDS];
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
            //keywords[i] = wordsFreq[i].first; // don't store count, only words
            keywords [i] = Cadena ((char *) wordsFreq [i].first.c_str());
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

    diskManager pm1 = 
        std::make_shared<DiskManager> (treefile, false);
    diskManager pm2 = 
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
                //str keywords[MAX_KEYWORDS];
                Cadena keywords [MAX_KEYWORDS];
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

std::map <size_t, int> SearchEngineManager::search_word (const str word)
{
    if (word.empty ()){
        std::cerr << "You need to provide a word to search" << std::endl;
        exit(1);
    }

    diskManager pm1 = 
        std::make_shared<DiskManager> (treefile, false);
    diskManager pm2 = 
        std::make_shared<DiskManager> (wordsfile, false);
    std::shared_ptr<DiskManager> kwpm = 
        std::make_shared<DiskManager> (keywordfile, false);

    bptree mtree (pm1);
    Cadena cad ((char *) word.c_str());

    int id = mtree.search (cad);

    std::map <size_t, int> res;

    if (id != -1) {
        std::pair<int, int> results [MAX_ARTICLES];
        pm2->retrieve_record (id, results);
        for (int i = 0; i < MAX_ARTICLES; i++){
            const size_t artId = results[i].first;       
            if (artId > 0) {
                res [artId] = results[i].second;
            }     
        }
    }
    return res;
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

std::set<size_t> SearchEngineManager::search(const str text)
{
    std::set<size_t> matches;

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
                matches.insert(results[i].first);
        }
    }
    
    return matches;
}

std::vector<size_t> SearchEngineManager::ranked_search(const str text){
    //std::set<size_t> results = search(text);
    preprocessor.setText(text);
    std::map<size_t, double> m_rank;
    double tf_idf;

    auto all_words = preprocessor.getWordCount ();
    double max_n = all_words.size ();
    for (auto &term: all_words) {
        std::map <size_t, int> words_counts = search_word (term.first);
        double df = words_counts.size ();
        for (auto &doc: words_counts) {
            double tf = doc.second;
            tf_idf = double (log10(1 + tf));
            tf_idf *= double (log10(max_n / df));
            m_rank [doc.first] = tf_idf;
        }
    }
/*
    for(auto&term:preprocessor.getWordCount()){
        for(auto&document:results){
            tf_idf = tfidf(document, term.first, results);
            m_rank[document] += tf_idf;
            //std::cout << term.first << 'x' << document << ':' << m_rank[document] << '\n';
        }
    }*/

    std::vector<std::pair<size_t, double>> rank;
    for(auto&p:m_rank)
        rank.push_back(p);

    std::sort(rank.begin(), rank.end(), compare);

    int max_size = (MAX_ARTICLES < rank.size()) ? MAX_ARTICLES: rank.size();
    std::vector<size_t> top_results(max_size);

    for(int i=0;i<max_size;++i)
        top_results[i] = rank[i].first;

    return top_results;
}

std::vector<std::string> SearchEngineManager::getArticleKeywords(size_t idx)
{
    std::vector<str> kws;
    std::shared_ptr<DiskManager> kwpm = 
    std::make_shared<DiskManager> (keywordfile, false);

    Cadena keywords[MAX_KEYWORDS];
    kwpm->retrieve_record (idx, keywords);
    for (int i=0; i<MAX_KEYWORDS; ++i) {
        str temp (keywords [i].get ());
        
        if (temp.size())
            kws.push_back(temp);
    }
    return kws;
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
