#ifndef __JSON_UTILS_HEADER__
#define __JSON_UTILS_HEADER__

#include <crow.h>
#include <string.h>
#include <mysql++/mysql++.h>
// https://tangentsoft.com/mysqlpp/doc/html/refman/classmysqlpp_1_1Connection.htmli
// Connection (const char *db, const char *server=0, const char *user=0, const char *password=0, unsigned int port=0)
// example :  mysqlpp::Connection conn("test_rest_DB", "mysqlserver", "dev_admin", "dev_admin");

#ifndef CONNECTION_STRUCT
#define CONNECTION_STRUCT
struct mysql_connection_s
{
    const char* db;
    const char* server;
    const char* user;   
    const char* password;
    unsigned int port;
};
typedef struct mysql_connection_s mysql_connection_t;
#endif

typedef void (*mysql_func_ptr_t) (crow::SimpleApp&, mysqlpp::Connection&);

namespace mysql_helpers
{
    
    int simple_api_exec(const mysql_connection_t* conn_id, mysql_func_ptr_t func_ptr_arr[], int port);
    const mysql_connection_t* allocate_mysql_credentials(const char* db, const char* server=0, const char* user=0, const char* password=0, unsigned int port=0);
    int free_mysql_credentials(mysql_connection_t* connection_param);
    std::string create_key_string(const crow::json::rvalue& data);
    std::string create_value_row_string(const crow::json::rvalue& row);
    std::string create_values_string(const crow::json::rvalue& data);
    std::string crow_json_converter(const crow::json::rvalue& val);
    std::string create_update_fields_string(const crow::json::rvalue& data);
}


#endif
