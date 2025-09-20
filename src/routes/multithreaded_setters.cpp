
#include <rest_api.hpp>
#include <unordered_map>
#include <mysql_helpers.hpp>
#include <db_repository.hpp>

using namespace mysql_helpers;

// std::string create_update_fields_string(const crow::json::rvalue& data)
// {
//     std::string result;
//     auto it = data.begin();
//     auto end = data.end();
//     while (it != end)
//     {
//         const auto& val = *it;
//         result += "`" + std::string(it->key()) + "`=";
//         result += crow_json_converter(val);
//         ++it;
//         if (it != end)
//         {
//             result += ", ";
//         }
//     }
//     return result;
// }

// INSERT 
void thread_safe_crow_insert_entity(crow::SimpleApp& app, SimpleConnectionPool& pool_ptr)
{
    CROW_ROUTE(app, "/insert/<string>").methods(crow::HTTPMethod::POST)
    ([&pool_ptr](const crow::request& req, const std::string& table_name)
    {
        mysqlpp::ScopedConnection sc(pool_ptr, true);
        mysql_repository repo(sc);
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
void thread_safe_crow_update_entity_by_id(crow::SimpleApp& app, SimpleConnectionPool& pool_ptr)
{
    CROW_ROUTE(app, "/update/<string>/<int>").methods(crow::HTTPMethod::PUT)
    ([&pool_ptr](const crow::request& req, const std::string& table_name, int id)
    {
        mysqlpp::ScopedConnection sc(pool_ptr, true);
        mysql_repository repo(sc);
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
void  thread_safe_crow_delete_entity_by_id(crow::SimpleApp& app, SimpleConnectionPool& pool_ptr)
{
    CROW_ROUTE(app, "/delete/<string>/<int>").methods(crow::HTTPMethod::DELETE)
    ([&pool_ptr](const std::string& table_name, int id)
    {
        mysqlpp::ScopedConnection sc(pool_ptr, true);
        mysql_repository repo(sc);
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

void  thread_safe_crow_set(crow::SimpleApp& app, sw::redis::Redis& redis)
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

void  thread_safe_crow_lpush(crow::SimpleApp& app, sw::redis::Redis& redis)
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

void  thread_safe_crow_rpush(crow::SimpleApp& app, sw::redis::Redis& redis)
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

void  thread_safe_crow_hmset(crow::SimpleApp& app, sw::redis::Redis& redis)
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



