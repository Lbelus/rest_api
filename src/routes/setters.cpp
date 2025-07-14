#include <rest_api.hpp>


// INSERT 

void crow_create_entity(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/<string>").methods(crow::HTTPMethod::POST)
    ([&mysql](const crow::request& req, const std::string& entity)
    {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        std::string name = body["name"].s();
        int value = body["value"].i();

        std::ostringstream oss;
        oss << "INSERT INTO " << mysqlpp::quote_only << entity
            << " (name, value) VALUES ("
            << mysqlpp::quote << name << ", "
            << mysqlpp::quote << value << ")";
        mysqlpp::Query query = mysql.query(oss.str());

        if (query.execute()) {
            return crow::response(201, "Entity created");
        } else {
            return crow::response(500, "Insert failed");
        }
    });
}


// UPDATE

void crow_update_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/<string>/<int>").methods(crow::HTTPMethod::PUT)
    ([&mysql](const crow::request& req, const std::string& entity, int id)
    {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        std::string name = body["name"].s();
        int value = body["value"].i();

        std::ostringstream oss;
        oss << "UPDATE " << mysqlpp::quote_only << entity
            << " SET name=" << mysqlpp::quote << name
            << ", value=" << mysqlpp::quote << value
            << " WHERE id=" << mysqlpp::quote << id;
        mysqlpp::Query query = mysql.query(oss.str());

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
    CROW_ROUTE(app, "/<string>/<int>").methods(crow::HTTPMethod::DELETE)
    ([&mysql](const std::string& entity, int id)
    {
        std::ostringstream oss;
        oss << "DELETE FROM " << mysqlpp::quote_only << entity
            << " WHERE id=" << mysqlpp::quote << id;

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

// JOIN
void crow_get_joined_entities(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/<string>/join/<string>").methods(crow::HTTPMethod::GET)
    ([&mysql](const std::string& entity1, const std::string& entity2)
    {
        // Example: JOIN on `entity1.id = entity2.<entity1>_id`
        std::string join_condition = entity1 + ".id = " + entity2 + "." + entity1 + "_id";

        std::ostringstream oss;
        oss << "SELECT * FROM " << mysqlpp::quote_only << entity1
            << " JOIN " << mysqlpp::quote_only << entity2
            << " ON " << entity1 << ".id = " << entity2 << "." << entity1 << "_id";
        mysqlpp::Query query = mysql.query(oss.str());

        if (auto result = query.store())
        {
            std::ostringstream oss;
            oss << result;
            return crow::response(200, oss.str());
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
    CROW_ROUTE(app, "/<string>/order/<string>").methods(crow::HTTPMethod::GET)
    ([&mysql](const std::string& entity, const std::string& column)
    {
        std::ostringstream oss;
        oss << "SELECT * FROM " << mysqlpp::quote_only << entity
            << " ORDER BY " << column << " ASC";
        mysqlpp::Query query = mysql.query(oss.str());

        if (auto result = query.store())
        {
            std::ostringstream oss;
            oss << result;
            return crow::response(200, oss.str());
        }
        else
        {
            return crow::response(500, "Order query failed");
        }
    });
}


