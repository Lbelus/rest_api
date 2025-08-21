#include <rest_api.hpp>
#include <unordered_map>
#include <mysql_utils.hpp>
#include <db_repository.hpp>
using namespace mysql_utils;

std::string create_update_fields_string(const crow::json::rvalue& data)
{
    std::string result;
    auto it = data.begin();
    auto end = data.end();
    while (it != end)
    {
        const auto& val = *it;
        result += "`" + std::string(it->key()) + "`=";
        result += crow_json_converter(val);
        ++it;
        if (it != end)
        {
            result += ", ";
        }
    }
    return result;
}

// INSERT 
void crow_insert_entity(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/insert/<string>").methods(crow::HTTPMethod::POST)
    ([&mysql](const crow::request& req, const std::string& table_name)
    {
        mysql_repository repo(mysql);
        crow::json::rvalue data = crow::json::load(req.body);
        if (!data)
        {
            return crow::response(400, "Invalid JSON - expected list of objects");
        } 
        const std::string& keys = create_key_string(data[0]);
        const std::string& values = create_values_string(data);
        bool result = repo.insert(table_name, keys, values);
        if (result)
        {
            return crow::response(500, repo.error());
        }
        return crow::response(201, "Entity created");
    });
}

// UPDATE
void crow_update_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/update/<string>/<int>").methods(crow::HTTPMethod::PUT)
    ([&mysql](const crow::request& req, const std::string& table_name, int id)
    {
        mysql_repository repo(mysql);
        crow::json::rvalue data = crow::json::load(req.body);
        if (!data)
        {
            return crow::response(400, "Missing or invalid JSON");
        } 
        std::string fields_string = create_update_fields_string(data);
        bool result = repo.update(table_name, fields_string, id);
        if (result)
        {
            return crow::response(500, repo.error());
        }
        return crow::response(201, "Entity updated");
    });
}

// DELETE
void crow_delete_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/delete/<string>/<int>").methods(crow::HTTPMethod::DELETE)
    ([&mysql](const std::string& table_name, int id)
    {
        mysql_repository repo(mysql);
        bool result = repo.delete__(table_name, id);
        if (result)
        {
            return crow::response(500, repo.error());
        }
        return crow::response(201, "Entity deleted");
    });
}


/// REDIS IS CURRENTLY OUT OF SCOPE

//redis 

void crow_set(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/set").methods(crow::HTTPMethod::POST)
    ([&redis](const crow::request& req)
    {
        auto x = crow::json::load(req.body);
        if (!x)
        {
            return crow::response(400, "Invalid JSON");
        }

        std::string key = x["key"].s();
        std::string value = x["value"].s();

        redis.set(key, value);

        return crow::response(200, "Data stored successfully");
    });
}

void crow_lpush(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/lpush").methods(crow::HTTPMethod::POST)
    ([&redis](const crow::request& req)
    {
        auto x = crow::json::load(req.body);
        if (!x)
        {
            return crow::response(400, "Invalid JSON");
        }

        std::string key = x["key"].s();
        std::string value = x["value"].s();

        redis.lpush(key, value);

        return crow::response(200, "Data stored successfully");
    });
}

void crow_rpush(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/rpush").methods(crow::HTTPMethod::POST)
    ([&redis](const crow::request& req)
    {
        auto x = crow::json::load(req.body);
        if (!x)
        {
            return crow::response(400, "Invalid JSON");
        }

        std::string key = x["key"].s();
        std::string value = x["value"].s();

        redis.rpush(key, value);

        return crow::response(200, "Data stored successfully");
    });
}

void crow_hmset(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/hmset").methods(crow::HTTPMethod::POST)
    ([&redis](const crow::request& req)
    {
        auto x = crow::json::load(req.body);
        if (!x)
        {
            return crow::response(400, "Invalid JSON");
        }

        std::string hash = x["key"].s();
        std::unordered_map<std::string, std::string> fields_values;

        const auto& fields = x["fields_values"];
        for (auto it = fields.begin(); it != fields.end(); ++it)
        {
            fields_values[it->key()] = it->s();
        }

        redis.hmset(hash, fields_values.begin(), fields_values.end());

        return crow::response(200, "Hash data stored successfully");
    });
}



