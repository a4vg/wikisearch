#include <crow.h>
#include <json.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "SearchEngineManager.h"
#include "ZimManager.h"

SearchEngineManager sem("../DataProcessing/data/zim/wiki-full.zim");
ZimManager zimmanager("../DataProcessing/data/zim/wiki-full.zim");

void getJsonFromSearch(nlohmann::json &articlesJson, std::string query)
{
    auto start = std::chrono::system_clock::now();
    auto matchesId = sem.ranked_search(query);
    auto end = std::chrono::system_clock::now();

    double time = std::chrono::duration_cast<std::chrono::milliseconds >( end - start).count();

    for (size_t id: matchesId)
    {
        nlohmann::json article;
        article["id"] = id;
        article["title"] = zimmanager.getArticleTitle(id);

        auto it = zimmanager.getIteratorFromArticleId(id);
        article["description"] = (*it).second;

        article["keywords"] = sem.getArticleKeywords(id);

        article["time"] = time/1000;

        articlesJson.push_back(article);
    }
}

crow::response responseWithCors(std::string x)
{
    auto r = crow::response(x);
    r.add_header("Access-Control-Allow-Origin", "*");
    return r;
}

std::string error(const char* e, nlohmann::json info = {})
{
    nlohmann::json jsonErr = nlohmann::json({{"error", e}});
    if (!info.empty())
        jsonErr["info"] = info;
    return jsonErr.dump();
}


int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/search")
    ([](const crow::request& req) {
        auto q = req.url_params.get("q");
        if (!q)
            return responseWithCors(error("Missing parameter q"));

        nlohmann::json articlesJson = nlohmann::json::array();
        getJsonFromSearch(articlesJson, std::string(q));
        return responseWithCors(articlesJson.dump());
    });

    CROW_ROUTE(app, "/article/<int>")
    ([](int idx) {
        nlohmann::json article;
        article["id"] = idx;

        if (idx < 0)
            return responseWithCors(error("Invalid article ID", article));

        try {
            if (!zimmanager.isValidArticle(idx))
                return responseWithCors(error("ID doesn't belong to an article", article));
            article["title"] = zimmanager.getArticleTitle(idx);
            article["html"] = zimmanager.getArticleHtml(idx);
        } catch (const std::exception& e) {
            return responseWithCors(error("Invalid article ID: Index out of range", article));
        }

        return responseWithCors(zimmanager.getArticleHtml(idx));
    });

    app.port(18080).run();
}
