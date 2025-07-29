#include <rest_api.hpp>
#include <unordered_map>
#include <mock_querry_builder.hpp>

std::string create_key_string(const crow::json::rvalue& data)
{
    std::string result = "(";
    auto it = data.begin();
    auto end = data.end();
    while (it != end)
    {
        result += "`" + std::string(it->key()) + "`";
        ++it;
        if (it != end)
        {
            result += ", ";
        }
    }
    result += ")";
    return result;
}

std::string create_value_row_string(const crow::json::rvalue& row)
{
    std::string result = "(";
    auto it = row.begin();
    auto end = row.end();
    while (it != end)
    {
        result += "'" + std::string((*it).s()) + "'";
        ++it;
        if (it != end)
        {
            result += ", ";
        }
    }
    result += ")";
    return result;
}

std::string create_values_string(const crow::json::rvalue& data)
{
    std::string result;
    auto it = data.begin();
    auto end = data.end();
    while (it != end)
    {
        result += create_value_row_string(*it);
        ++it; 
        if (it != end)
        {
            result += ", ";
        }
    }
    // msg += ";";
    return result;
}
// INSERT 
void crow_create_entity(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/create/<string>").methods(crow::HTTPMethod::POST)
    ([&mysql](const crow::request& req, const std::string& table_name)
    {
        crow::json::rvalue data = crow::json::load(req.body);
        if (!data)
        {
            return crow::response(400, "Invalid JSON - expected list of objects");
        }

        std::string msg("INSERT INTO `" + table_name + "` ");

        // Get keys from first object only
        msg += create_key_string(data[0]);
        msg += " VALUES ";
        msg += create_values_string(data);
        CROW_LOG_INFO << "SQL: " << msg;
        mysqlpp::Query query = mysql.query(msg);

        if (query.execute())
        {
            return crow::response(201, "Entity created");
        }
        else
        {
            return crow::response(500, query.error());
        }
    });
}

// UPDATE
void crow_update_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/update/<string>/<int>").methods(crow::HTTPMethod::PUT)
    ([&mysql](const crow::request& req, const std::string& entity, int id)
    {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("name")) {
            return crow::response(400, "Missing or invalid JSON");
        }

        std::string name = body["name"].s();

        std::ostringstream oss;
        oss << "UPDATE `" << entity << "` "
            << "SET `name`='" << name
            << "' WHERE `id`=" << id;

        CROW_LOG_INFO << "SQL: " << oss.str();

        mysqlpp::Query query = mysql.query(oss.str());

        if (!query) {
            return crow::response(500, query.error());
        }

        if (query.execute()) {
            return crow::response(200, "Entity updated");
        } else {
            return crow::response(404, "Update failed or entity not found");
        }
    });
}


// DELETE
void crow_delete_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/delete/<string>/<int>").methods(crow::HTTPMethod::DELETE)
    ([&mysql](const std::string& entity, int id)
    {
        std::ostringstream oss;
        oss << "DELETE FROM `" << entity
            << "` WHERE `id`=" << id;

        mysqlpp::Query query = mysql.query(oss.str());

        if (query.execute())
        {
            return crow::response(200, "Entity deleted");
        }
        else
        {
            return crow::response(404, "Entity not found or delete failed");
        }
    });
}



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



