#include <rest_api.hpp>
#include <mock_querry_builder.hpp>

// FROM SELECT WHERE
// we are at the MVP stage security and validation is not an immediate concern. 


auto test_print(auto result)
{
    std::ostringstream out;
    for (const auto& row : result)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            out << row[i];
            if (i != row.size() - 1)
                out << ", ";
        }
        out << "\n";
    }
    return out;
}


void crow_get_all_entity(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/read/<string>").methods(crow::HTTPMethod::GET)
    ([&mysql](const std::string& key)
    {
        std::ostringstream oss;
        oss << "SELECT * FROM " << '`' << key << '`';
        mysqlpp::Query query = mysql.query(oss.str());

        if (auto result = query.store())
        {
            std::ostringstream out;
            // out << result;
            out = test_print(result);
            return crow::response(200, out.str());
        }
        else
        {
            return crow::response(404, "Entity not found");
        }
    });
}

void crow_get_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/read/<string>/<int>").methods(crow::HTTPMethod::GET)
    ([&mysql](const std::string& key, int id)
    {
        std::ostringstream oss;
        oss << "SELECT * FROM `" << key << "` WHERE id=" << id;

        mysqlpp::Query query = mysql.query(oss.str());

        if (auto result = query.store())
        {
            std::ostringstream out;
            out = test_print(result);
            // out << result;
            return crow::response(200, out.str());
        }
        else
        {
            return crow::response(404, "Entity not found");
        }
    });
}

// JOIN
void crow_get_joined_entities(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/join/<string>/<string>").methods(crow::HTTPMethod::GET)
    ([&mysql](const std::string& entity1, const std::string& entity2)
    {
        // Join condition: `entity1`.`id` = `entity2`.`entity1_id`
        std::ostringstream oss;
        oss << "SELECT * FROM `" << entity1 << "` "
            << "JOIN `" << entity2 << "` "
            << "ON `" << entity1 << "`.`id` = `" << entity2 << "`.`" << entity1 << "_id`";

        CROW_LOG_INFO << "SQL: " << oss.str();
        mysqlpp::Query query = mysql.query(oss.str());

        if (!query)
            return crow::response(500, query.error());

        if (auto result = query.store())
        {
            std::ostringstream out;
            out = test_print(result);
            return crow::response(200, out.str());
        }
        else
        {
            return crow::response(500, "Join failed");
        }
    });
}

// ORDER
void crow_get_ordered_entities(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/order/<string>/<string>").methods(crow::HTTPMethod::GET)
    ([&mysql](const std::string& entity, const std::string& column)
    {
        std::ostringstream oss;
        oss << "SELECT * FROM `" << entity << "` "
            << "ORDER BY `" << column << "` ASC";

        CROW_LOG_INFO << "SQL: " << oss.str();

        mysqlpp::Query query = mysql.query(oss.str());

        if (!query)
            return crow::response(500, query.error());

        if (auto result = query.store())
        {
            std::ostringstream out;
            out = test_print(result);
            return crow::response(200, out.str());
        }
        else
        {
            return crow::response(500, "Order query failed");
        }
    });
}


void crow_key(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/key/<string>").methods(crow::HTTPMethod::GET)
    ([&redis](const std::string& key)
    {
        auto val = redis.get(key);
        if (val)
        {
            return crow::response(200, *val);
        }
        else
        {
            return crow::response(404, "Data not found");
        }
    });
}

void crow_get(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/get/<string>").methods(crow::HTTPMethod::GET)
    ([&redis](const std::string& key)
    {
        auto val = redis.get(key);
        if (val)
        {
            return crow::response(200, *val);
        }
        else
        {
            return crow::response(404, "Data not found");
        }
    });
}


void crow_hmget(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/hmget").methods(crow::HTTPMethod::POST)
    ([&redis](const crow::request& req)
    {
        auto x = crow::json::load(req.body);
        if (!x)
        {
            return crow::response(400, "Invalid JSON");
        }

        std::string hash = x["key"].s();
        std::vector<std::string> fields;
        
        for (const auto& item : x["fields"])
        {
            fields.push_back(item.s());
        }

        std::vector<sw::redis::OptionalString> vals;
        redis.hmget(hash, fields.begin(), fields.end(), std::back_inserter(vals));
        crow::json::wvalue result; // https://www.ccoderun.ca/programming/doxygen/Crow/classcrow_1_1json_1_1wvalue.html
        for (size_t i = 0; i < fields.size(); ++i)
        {
            if (vals[i])
            {
                result[fields[i]] = *vals[i];
            }
            else
            {
                result[fields[i]] = nullptr;
            }
        }
        return crow::response(200, result);
    });
}


void crow_lpop(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/lpop/<string>").methods(crow::HTTPMethod::GET)
    ([&redis](const std::string& key)
    {
        auto val = redis.lpop(key);
        if (val)
        {
            return crow::response(200, *val);
        }
        else
        {
            return crow::response(404, "Data not found");
        }
    });
}

void crow_rpop(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/rpop/<string>").methods(crow::HTTPMethod::GET)
    ([&redis](const std::string& key)
    {
        auto val = redis.rpop(key);
        if (val)
        {
            return crow::response(200, *val);
        }
        else
        {
            return crow::response(404, "Data not found");
        }
    });
}

void crow_llen(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/llen/<string>").methods(crow::HTTPMethod::GET)
    ([&redis](const std::string& key)
    {
        long long val = redis.llen(key);
        if (val)
        {
            std::string str = std::to_string(val);
            return crow::response(200, str);
        }
        else
        {
            return crow::response(404, "List is empty");
        }
    });
}


void crow_ping(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/ping").methods(crow::HTTPMethod::GET)
    ([&redis]()
    {
        std::string val = redis.ping();
        if (!val.empty())
        {
            return crow::response(200, val);
        }
        else
        {
            return crow::response(404, "server unresponsive");
        }
    });
}

void crow_echo(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/echo/<string>").methods(crow::HTTPMethod::GET)
    ([&redis](const std::string& msg)
    {
        std::string val = redis.echo(msg);
        if (!val.empty())
        {
            return crow::response(200, val);
        }
        else
        {
            return crow::response(404, "server unresponsive");
        }
    });
}

void crow_flushall(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/flushall").methods(crow::HTTPMethod::GET)
    ([&redis]()
    {
        redis.flushall();
        return crow::response(200, "data has been flushed");
    });
}


void crow_info(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/info").methods(crow::HTTPMethod::GET)
    ([&redis]()
    {
        std::string  val = redis.info();
        if (!val.empty())
        {
            return crow::response(200, val);
        }
        else
        {
            return crow::response(404, "Redis could not provide any info");
        }
    });
}


