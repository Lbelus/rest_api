#include <rest_api.hpp>
#include <vector>
#include <db_repository.hpp>
#include <mysql_conn_pool.hpp>

// #include <mock_querry_builder.hpp>

// FROM SELECT WHERE
// we are at the MVP stage security and validation is not an immediate concern. 
// auto test_print(const std::vector<mysqlpp::Row>& result)
// {
//     std::ostringstream out;
//     for (const auto& row : result)
//     {
//         out << "row: ";

//         for (size_t i = 0; i < row.size(); ++i)
//         {
//             
//             out << row[(int)i] << (i + 1 < row.size() ? ", " : "");
//         }
//         out << "\n";
//     }
//     return out;
// }

// crow::json::wvalue to_crow_json(const std::vector<mysqlpp::Row>& rows,
//                                 const std::vector<std::string>& names)
// {
//     crow::json::wvalue::list arr;
//     for (const auto& row : rows)
//     {
//         crow::json::wvalue obj;
//         for (size_t index = 0; index < names.size(); ++index)
//         {
//             int pos = (int)index;
//             if (row == NULL)
//             {
//                 obj[names[pos]] = nullptr;
//             }
//             else
//             {
//                 obj[names[pos]] = std::string(row[pos].c_str());
//             }
//         }
//         arr.push_back(std::move(obj));
//     }
//     return crow::json::wvalue(std::move(arr));
// }

void thread_safe_crow_get_all_entity(crow::SimpleApp& app, SimpleConnectionPool& pool_ptr)
{
    CROW_ROUTE(app, "/read/<string>").methods(crow::HTTPMethod::GET)
    ([&pool_ptr](const std::string& key)
    {
        mysqlpp::ScopedConnection sc(pool_ptr, true);
        mysql_repository repo(sc);
        bool result = repo.select_all(key);
        if (!result)
        { 
            const std::vector<mysqlpp::Row>& rows = repo.get_rows();
            const std::vector<std::string>& names = repo.get_names();
            crow::json::wvalue res = mysql_helpers::to_crow_json(rows, names);
            return crow::response(200, res);
        }
        else
        {
            return crow::response(404, "Entity not found");
        }
    });
}

// void crow_get_entry(crow::SimpleApp& app, SimpleConnectionPool& pool_ptr)
// {
//     CROW_ROUTE(app, "/read/<string>/<int>").methods(crow::HTTPMethod::GET)
//     ([&mysql](const std::string& key, int id)
//     {
//         mysql_repository repo(pool_ptr);
//         bool result = repo.select_all(key);
//         if (!result)
//         { 
//             const std::vector<mysqlpp::Row>& rows = repo.get_rows();
//             const std::vector<std::string>& names = repo.get_names();
//             crow::json::wvalue res = to_crow_json(rows, names);
//             return crow::response(200, res);
//         }
//         else
//         {
//             return crow::response(404, "Entity not found");
//         }
//     });
// }

void thread_safe_crow_get_entity_by_id(crow::SimpleApp& app, SimpleConnectionPool& pool_ptr)
{
    CROW_ROUTE(app, "/read/<string>/<int>").methods(crow::HTTPMethod::GET)
    ([&pool_ptr](const std::string& key, int id)
    {
         
        mysqlpp::ScopedConnection sc(pool_ptr, true);
        mysql_repository repo(sc);
        bool result = repo.select_by_id(key, id);
        if (!result)
        { 
            const std::vector<mysqlpp::Row>& rows = repo.get_rows();
            const std::vector<std::string>& names = repo.get_names();
            crow::json::wvalue res = mysql_helpers::to_crow_json(rows, names);
            return crow::response(200, res);
        }
        else
        {
            return crow::response(404, "Entity not found");
        }
    });
}

// JOIN
void thread_safe_crow_get_joined_entities(crow::SimpleApp& app, SimpleConnectionPool& pool_ptr)
{
    CROW_ROUTE(app, "/join/<string>/<string>").methods(crow::HTTPMethod::GET)
    ([&pool_ptr](const std::string& table_name_A, const std::string& table_name_B)
    {
        mysqlpp::ScopedConnection sc(pool_ptr, true);
        mysql_repository repo(sc);
        bool result = repo.join(table_name_A, table_name_B);
        if (!result)
        { 
            const std::vector<mysqlpp::Row>& rows = repo.get_rows();
            const std::vector<std::string>& names = repo.get_names();
            crow::json::wvalue res = mysql_helpers::to_crow_json(rows, names);
            return crow::response(200, res);
        }
        else
        {
            return crow::response(404, "Entity not found");
        }
    });
}

// ORDER
void thread_safe_crow_get_ordered_entities(crow::SimpleApp& app, SimpleConnectionPool& pool_ptr)
{
    CROW_ROUTE(app, "/order/<string>/<string>/<string>").methods(crow::HTTPMethod::GET)
    ([&pool_ptr](const std::string& table_name, const std::string& column, const std::string& order)
    {
        mysqlpp::ScopedConnection sc(pool_ptr, true);
        mysql_repository repo(sc);
        bool result = repo.order(table_name, column, order);
        if (!result)
        { 
            const std::vector<mysqlpp::Row>& rows = repo.get_rows();
            const std::vector<std::string>& names = repo.get_names();
            crow::json::wvalue res = mysql_helpers::to_crow_json(rows, names);
            return crow::response(200, res);
        }
        else
        {
            return crow::response(404, "Entity not found");
        }
    });
}


// REDIS


void thread_safe_crow_key(crow::SimpleApp& app, sw::redis::Redis& redis)
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

void thread_safe_crow_get(crow::SimpleApp& app, sw::redis::Redis& redis)
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


void thread_safe_crow_hmget(crow::SimpleApp& app, sw::redis::Redis& redis)
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


void thread_safe_crow_lpop(crow::SimpleApp& app, sw::redis::Redis& redis)
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

void thread_safe_crow_rpop(crow::SimpleApp& app, sw::redis::Redis& redis)
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

void thread_safe_crow_llen(crow::SimpleApp& app, sw::redis::Redis& redis)
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


void thread_safe_crow_ping(crow::SimpleApp& app, sw::redis::Redis& redis)
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

void thread_safe_crow_echo(crow::SimpleApp& app, sw::redis::Redis& redis)
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

void thread_safe_crow_flushall(crow::SimpleApp& app, sw::redis::Redis& redis)
{
    CROW_ROUTE(app, "/flushall").methods(crow::HTTPMethod::GET)
    ([&redis]()
    {
        redis.flushall();
        return crow::response(200, "data has been flushed");
    });
}

void thread_safe_crow_info(crow::SimpleApp& app, sw::redis::Redis& redis)
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


