#ifndef DB_REPOSITORY_HPP_
#define DB_REPOSITORY_HPP_

#include <mysql++/mysql++.h>
#include <string>

class mysql_repository 
{
    private:
        mysqlpp::Connection& conn;

    public:
        mysql_repository(mysqlpp::Connection conn): conn(conn) {}

        //C
        bool create(const std::string& table_name);
        //R
        bool select_all(const std::string& key);
        bool select_by_id(const std::string& key, int id);
        bool join(const std::string& table_name_A, const std::string& table_name_B);
        bool order(const std::string& table_name, const std::string& column, const std::string& order);

        //U
        bool update(const std::string& table_name, const std::string& fields_string, int id);
        //D
        bool delete__(const std::string& table_name, int id);

};


#endif
