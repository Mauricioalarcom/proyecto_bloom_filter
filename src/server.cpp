#include "BloomFilter.h"
#include "crow_all.h"

#include <fstream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace {

void addCorsHeaders(crow::response &resp) {
    resp.add_header("Access-Control-Allow-Origin", "*");
    resp.add_header("Access-Control-Allow-Headers", "Content-Type");
}

bool loadBlacklist(const std::string &path, BloomFilter &filter,
                                     std::unordered_set<std::string> &exactSet) {
    std::ifstream input(path);
    if (!input.is_open()) {
        return false;
    }

    std::string url;
    size_t count = 0;
    while (std::getline(input, url)) {
        if (url.empty()) {
            continue;
        }
        filter.insert(url);
        exactSet.insert(url);
        ++count;
    }

    CROW_LOG_INFO << "Cargadas " << count << " URLs desde " << path;
    return true;
}

} // namespace

int main() {
    crow::SimpleApp app;

    auto filter = std::make_shared<BloomFilter>(1'000'000, 7);
    auto exactSet = std::make_shared<std::unordered_set<std::string>>();

    const std::vector<std::string> candidates = {
            "tests/blacklist_urls.txt", "../tests/blacklist_urls.txt",
            "blacklist_urls.txt"};

    bool loaded = false;
    for (const auto &candidate : candidates) {
        if (loadBlacklist(candidate, *filter, *exactSet)) {
            loaded = true;
            break;
        }
    }

    if (!loaded) {
        CROW_LOG_ERROR << "No se encontró blacklist_urls.txt; el servidor arrancará con estructuras vacías.";
    }

    CROW_ROUTE(app, "/check").methods(crow::HTTPMethod::Post)([filter](
            const crow::request &req) {
        crow::response resp;
        addCorsHeaders(resp);
        resp.add_header("Content-Type", "application/json");

        try {
            auto body = crow::json::load(req.body);
            if (!body || body["url"].t() != crow::json::type::String) {
                resp.code = 400;
                resp.body = R"({"malicious":false,"reason":"Missing or invalid 'url' in body"})";
                return resp;
            }

            const std::string url = body["url"].s();
            const bool malicious = filter->possiblyContains(url);

            crow::json::wvalue result;
            result["malicious"] = malicious;
            if (!malicious) {
                result["reason"] = "URL not found in blacklist";
            }

            resp.code = 200;
            resp.body = result.dump();
            return resp;
        } catch (const std::exception &ex) {
            CROW_LOG_ERROR << "Error en /check: " << ex.what();
            resp.code = 500;
            resp.body =
                    R"({"malicious":false,"reason":"Internal server error"})";
            return resp;
        }
    });

    CROW_ROUTE(app, "/check").methods(crow::HTTPMethod::Options)([](
            const crow::request &) {
        crow::response resp;
        addCorsHeaders(resp);
        resp.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        resp.add_header("Access-Control-Allow-Headers", "Content-Type");
        resp.code = 204;
        return resp;
    });

    CROW_ROUTE(app, "/api/verify")
            .methods(crow::HTTPMethod::Post)([exactSet](const crow::request &req) {
                crow::response resp;
                addCorsHeaders(resp);
                resp.add_header("Content-Type", "application/json");

                try {
                    auto body = crow::json::load(req.body);
                    if (!body || body["url"].t() != crow::json::type::String) {
                        resp.code = 400;
                        resp.body = R"({"malicious":false,"reason":"Missing or invalid 'url' in body","method":"verify"})";
                        return resp;
                    }

                    const std::string url = body["url"].s();
                    const bool malicious = exactSet->find(url) != exactSet->end();

                    crow::json::wvalue result;
                    result["malicious"] = malicious;
                    result["method"] = "verify";
                    if (!malicious) {
                        result["reason"] = "URL not found in real blacklist";
                    }

                    resp.code = 200;
                    resp.body = result.dump();
                    return resp;
                } catch (const std::exception &ex) {
                    CROW_LOG_ERROR << "Error en /api/verify: " << ex.what();
                    resp.code = 500;
                    resp.body = R"({"malicious":false,"reason":"Internal server error","method":"verify"})";
                    return resp;
                }
            });

    CROW_ROUTE(app, "/api/verify").methods(crow::HTTPMethod::Options)([](
            const crow::request &) {
        crow::response resp;
        addCorsHeaders(resp);
        resp.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        resp.add_header("Access-Control-Allow-Headers", "Content-Type");
        resp.code = 204;
        return resp;
    });

    CROW_ROUTE(app, "/add").methods(crow::HTTPMethod::Post)([filter, exactSet](
            const crow::request &req) {
        crow::response resp;
        addCorsHeaders(resp);
        resp.add_header("Content-Type", "application/json");

        auto body = crow::json::load(req.body);
        if (!body || body["url"].t() != crow::json::type::String) {
            resp.code = 400;
            resp.body = R"({"added":false,"reason":"Missing 'url' in body"})";
            return resp;
        }

        const std::string url = body["url"].s();
        filter->insert(url);
        exactSet->insert(url);

        resp.code = 200;
        resp.body = R"({"added":true})";
        return resp;
    });

    CROW_ROUTE(app, "/add").methods(crow::HTTPMethod::Options)([](
            const crow::request &) {
        crow::response resp;
        addCorsHeaders(resp);
        resp.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        resp.add_header("Access-Control-Allow-Headers", "Content-Type");
        resp.code = 204;
        return resp;
    });

    CROW_ROUTE(app, "/clear").methods(crow::HTTPMethod::Post)([filter, exactSet](
            const crow::request &) {
        filter->clear();
        exactSet->clear();

        crow::response resp;
        addCorsHeaders(resp);
        resp.add_header("Content-Type", "application/json");
        resp.code = 200;
        resp.body = R"({"cleared":true})";
        return resp;
    });

    CROW_ROUTE(app, "/clear").methods(crow::HTTPMethod::Options)([](
            const crow::request &) {
        crow::response resp;
        addCorsHeaders(resp);
        resp.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        resp.add_header("Access-Control-Allow-Headers", "Content-Type");
        resp.code = 204;
        return resp;
    });

    CROW_ROUTE(app, "/health").methods(crow::HTTPMethod::Get)([](
            const crow::request &) {
        crow::response resp;
        addCorsHeaders(resp);
        resp.add_header("Content-Type", "application/json");
        resp.code = 200;
        resp.body = R"({"status":"ok"})";
        return resp;
    });

    app.port(18080).multithreaded().run();
    return 0;
}
