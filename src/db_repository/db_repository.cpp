#include <db_repository.hpp>

// Query acts as a standard C++ output stream, so you can write data to it like you would to std::cout or std::ostringstream.
// The library includes stream manipulators that are type-aware.
// source: https://tangentsoft.com/mysqlpp/doc/html/userman/overview.html

    //C
    bool mysql_repository::create(const std::string& table_name)
    {
        mysqlpp::Query query = conn.query();
        query << "INSERT INTO `" << table_name << "` ";
        return query.execute();
    }
    //R
    bool mysql_repository::select_all(const std::string& key)
    {

        mysqlpp::Query query = conn.query();
        query << "SELECT * FROM `" << key << "`";
        return query.execute();
    }

    bool mysql_repository::select_by_id(const std::string& key, int id)
    {
        mysqlpp::Query query = conn.query();
        query << "SELECT * FROM `" << key << "` WHERE id=" << id;
        return query.execute();

    }

    bool mysql_repository::join(const std::string& table_name_A, const std::string& table_name_B)
    {
        mysqlpp::Query query = conn.query();
        query << "SELECT * FROM `" << table_name_A << "` " 
            << "JOIN `" << table_name_B << "` "
            << "ON `" << table_name_A << "`.`id` = `" << table_name_B << "`.`" << table_name_A << "_id`";
        return query.execute();
    }

    bool mysql_repository::order(const std::string& table_name, const std::string& column, const std::string& order)
    {
        mysqlpp::Query query = conn.query();
        query << "SELECT * FROM `" << table_name + "` "
            << "ORDER BY `" << column << "` " << order;
        return query.execute();
    }

    //U
    bool mysql_repository::update(const std::string& table_name, const std::string& fields_string, int id)
    {
        mysqlpp::Query query = conn.query();
        query << "UPDATE `" + table_name + "` SET " << fields_string << " WHERE `id` = " << id;
        return query.execute();
    }

    //D
    bool mysql_repository::delete__(const std::string& table_name, int id)
    {
        mysqlpp::Query query = conn.query();
        query << "DELETE FROM `" << table_name << "` WHERE `id` =" << id;
        return query.execute();
    }
