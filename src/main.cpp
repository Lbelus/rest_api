#include <rest_api.hpp>
#include <mysql_helpers.hpp>


using namespace mysql_helpers;


int main()
{
    mysql_func_ptr_t func_ptr_arr[] =
    {
            &simple_crow_get_all_entity,
            &simple_crow_get_entity_by_id,
            &simple_crow_insert_entity,
            &simple_crow_update_entity_by_id,
            &simple_crow_delete_entity_by_id,
            &simple_crow_get_joined_entities,
            &simple_crow_get_ordered_entities,
            NULL
    };

    const mysql_connection_t* creds = allocate_mysql_credentials("test_rest_DB", "mysqlserver", "dev_admin", "dev_admin");
    SimpleConnectionPool pool(creds);
    simple_api_exec(creds, func_ptr_arr, 3004);
    free_mysql_credentials((mysql_connection_t*)creds);
    return EXIT_SUCCESS;
}
