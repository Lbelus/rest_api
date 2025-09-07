#include <mysql_utils.hpp>

//MySQL++ is a C++ wrapper around the MySQL C API (libmysqlclient).

// The MySQL C API ultimately calls mysql_real_connect(), which requires plaintext credentials.

// There is no built-in mechanism in MySQL++ to accept encrypted credentials or automatically decrypt them.

// Even if you hash or encrypt your credentials in your code, they must be decrypted to plaintext before passing to mysqlpp::Connection.


namespace mysql_utils
{
    int basic_api_exec(const mysql_connection_t* conn_id, mysql_func_ptr_t func_ptr_arr[], int port)
    {
        crow::SimpleApp app;
        mysqlpp::Connection conn(conn_id->db, conn_id->server, conn_id->user, conn_id->password);
        if (!mysqlpp::Connection::thread_aware())
        {
            std::cerr << "MySQL++/libmysqlclient not built thread-aware on this system\n";
            return 1;
        }
        size_t index = 0;
        while(func_ptr_arr[index] != NULL)
        {
            func_ptr_arr[index](app, conn);
            index += 1;
        }
        app.port(port).run();
        return EXIT_SUCCESS;
    }

    const char* allocate_string(const char* str)
    {
        char* new_str = NULL; 
        if (str != NULL)
        {
            size_t len = strlen(str) + 1;
            new_str = (char*)malloc(sizeof(char)* len);
            strcpy(new_str, str);
        }
        return (const char*)new_str;
    }

    const mysql_connection_t* allocate_mysql_credentials(
        const char* db, 
        const char* server, 
        const char* user, 
        const char* password, 
        unsigned int port)
    {
        mysql_connection_t* new_conn_param = (mysql_connection_t*)malloc(sizeof(mysql_connection_t));
        new_conn_param->db = allocate_string(db);
        new_conn_param->server = allocate_string(server);
        new_conn_param->user = allocate_string(user);
        new_conn_param->password = allocate_string(password);
        new_conn_param->port = port;
        return  (const mysql_connection_t*) new_conn_param;
    }
    
    int free_mysql_credentials(mysql_connection_t* connection_param)
    {
        free((void*)connection_param->db); 
        free((void*)connection_param->server); 
        free((void*)connection_param->user); 
        free((void*)connection_param->password);
        free(connection_param);
        return EXIT_SUCCESS;
    }
    
    std::string create_key_string(const crow::json::rvalue& data)
    {
        std::string result = "(";
        auto it = data.begin();
        auto end = data.end();
        while (it != end)
        {
            result += "`" + std::string(it->key()) + "`";
            ++it;
            if (it != end)
            {
                result += ", ";
            }
        }
        result += ")";
        return result;
    }

    std::string create_value_row_string(const crow::json::rvalue& row)
    {
        std::string result = "(";
        auto it = row.begin();
        auto end = row.end();
        while (it != end)
        {
            result += "'" + std::string((*it).s()) + "'";
            ++it;
            if (it != end)
            {
                result += ", ";
            }
        }
        result += ")";
        return result;
    }

    std::string create_values_string(const crow::json::rvalue& data)
    {
        std::string result;
        auto it = data.begin();
        auto end = data.end();
        while (it != end)
        {
            result += create_value_row_string(*it);
            ++it; 
            if (it != end)
            {
                result += ", ";
            }
        }
        // msg += ";";
        return result;
    }


    std::string crow_json_converter(const crow::json::rvalue& val)
    {
        std::string result;
        switch (val.t())
        {
            case crow::json::type::String:
                result += "'" + std::string(val.s()) + "'";
                break;
            case crow::json::type::Number:
                result += std::to_string(val.i());
                break;
            case crow::json::type::True:
                result += "TRUE";
                break;
            case crow::json::type::False:
                result += "FALSE";
                break;
            default:
                result += "NULL";
                break;
        }
        return result;
    }
};
