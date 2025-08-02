#include <rest_api.hpp>
#include <mysql_utils.hpp>
using namespace mysql_utils;


int main()
{
    mysql_func_ptr_t func_ptr_arr[] =
    {
            &crow_get_all_entity,
            &crow_get_entity_by_id,
            &crow_create_entity,
            &crow_update_entity_by_id,
            &crow_delete_entity_by_id,
            &crow_get_joined_entities,
            &crow_get_ordered_entities,
            NULL
    };

    const mysql_connection_t* creds = allocate_mysql_credentials("test_rest_DB", "mysqlserver", "dev_admin", "dev_admin");
    basic_api_exec(creds, func_ptr_arr, 3004);
    free_mysql_credentials((mysql_connection_t*)creds);
    return EXIT_SUCCESS;
}
