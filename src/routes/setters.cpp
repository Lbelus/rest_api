#include <rest_api.hpp>


// INSERT 
void crow_create_entity(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/create/<string>").methods(crow::HTTPMethod::POST)
    ([&mysql](const crow::request& req, const std::string& entity)
    {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("name")) {
            return crow::response(400, "Missing 'name' field");
        }

        std::string name = body["name"].s();

        std::ostringstream oss;
        oss << "INSERT INTO `" << entity
            << "` (`name`) VALUES ('"
            << name << "')";

        CROW_LOG_INFO << "SQL: " << oss.str();

        mysqlpp::Query query = mysql.query(oss.str());

        if (query.execute()) {
            return crow::response(201, "Entity created");
        } else {
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

// JOIN
void crow_get_joined_entities(crow::SimpleApp& app, mysqlpp::Connection& mysql)
{
    CROW_ROUTE(app, "/join/<string>/<string>").methods(crow::HTTPMethod::GET)
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
    CROW_ROUTE(app, "/order/<string>/<string>").methods(crow::HTTPMethod::GET)
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


