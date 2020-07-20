#include <crow.h>
#include <json.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "SearchEngineManager.h"
#include "ZimManager.h"

SearchEngineManager sem("../DataProcessing/data/zim/wiki-mini.zim");
ZimManager zimmanager("../DataProcessing/data/zim/wiki-mini.zim");

void getJsonFromSearch(nlohmann::json &articlesJson, std::string query)
{
    auto matchesId = sem.search(query);
    for (size_t id: matchesId)
    {
        nlohmann::json article;
        article["id"] = id;
        article["title"] = std::string(zimmanager.file.getArticle(id).getTitle());

        auto it = zimmanager.getIteratorFromArticleId(id);
        article["description"] = (*it).second;

        // TODO
        article["keywords"] = nlohmann::json::array();

        articlesJson.push_back(article);
    }
}


int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/search")
    ([](const crow::request& req) {
        auto q = req.url_params.get("q");
        if (!q)
        {
            auto r = crow::response(nlohmann::json({{"error", "Missing parameter q"}}).dump());
            r.add_header("Access-Control-Allow-Origin", "*");
            return r;
        }

        nlohmann::json articlesJson = nlohmann::json::array();
        getJsonFromSearch(articlesJson, std::string(q));
        auto r = crow::response(articlesJson.dump());
        r.add_header("Access-Control-Allow-Origin", "*");
        return r;
    });

    app.port(18080).run();
}
