#include <rest_api.hpp>

int main()
{
    crow::SimpleApp app;
    mysqlpp::Connection conn("test_rest_DB", "mysqlserver", "dev_admin", "dev_admin");

    crow_get_all_entity(app, conn);
    crow_get_entity_by_id(app, conn);
	crow_create_entity(app, conn);
	crow_update_entity_by_id(app, conn);
	crow_delete_entity_by_id(app, conn);
	crow_get_joined_entities(app, conn);
	crow_get_ordered_entities(app, conn);
    
    app.port(3004).multithreaded().run();

    return EXIT_SUCCESS;
}
