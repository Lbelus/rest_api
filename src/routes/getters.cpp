#include <rest_api.hpp>

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
