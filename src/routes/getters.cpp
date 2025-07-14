#include <rest_api.hpp>

// FROM SELECT WHERE
// we are at the MVP stage security and validation is not an immediate concern. 

void crow_get_all_entity(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/<string>").methods(crow::HTTPMethod::GET)
    ([&mysql](const std::string& key)
    {
        std::ostringstream oss;
        oss << "SELECT * FROM " << mysqlpp::quote_only << key;
        mysqlpp::Query query = mysql.query(oss.str());

        if (auto result = query.store())
        {
            std::ostringstream out;
            out << result;
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
    CROW_ROUTE(app, "/<string>/<int>").methods(crow::HTTPMethod::GET)
    ([&mysql](const std::string& key, int id)
    {
        std::ostringstream oss;
        oss << "SELECT * FROM " << mysqlpp::quote_only << key << " WHERE id=" << mysqlpp::quote << id;

        mysqlpp::Query query = mysql.query(oss.str());

        if (auto result = query.store())
        {
            std::ostringstream out;
            out << result;
            return crow::response(200, out.str());
        }
        else
        {
            return crow::response(404, "Entity not found");
        }
    });
}

