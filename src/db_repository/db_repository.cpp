#include <db_repository.hpp>

// Query acts as a standard C++ output stream, so you can write data to it like you would to std::cout or std::ostringstream.
// The library includes stream manipulators that are type-aware.
// source: https://tangentsoft.com/mysqlpp/doc/html/userman/overview.html

    //
    bool mysql_repository::insert(const std::string& table_name, const std::string& keys, const std::string& values)
    {
        mysqlpp::Query query = conn().query(); 
        query << "INSERT INTO `" << table_name << "` "
            << keys
            << " VALUES "
            << values; 
        simple_result = query.execute();
        if (simple_result)
        {
            return EXIT_SUCCESS;
        }
        else
        {
            error_msg = query.error();
            return EXIT_FAILURE;
        } 
    }

    void mysql_repository::store_names(const mysqlpp::StoreQueryResult& query_result)
    {
        names.clear();
        names.reserve(query_result.num_fields());
        size_t index = 0; 
        while (index < query_result.num_fields())
        {
            names.emplace_back(query_result.field_name(index));
            index += 1;
        } 
    }

    //R
    bool mysql_repository::select_all(const std::string& key)
    {
        mysqlpp::Query query = conn().query();
        query << "SELECT * FROM `" << key << "`";
        mysqlpp::StoreQueryResult query_result = query.store();
        if (query_result)
        {
            store_names(query_result);
            rows.assign(query_result.begin(), query_result.end());
            return EXIT_SUCCESS;
        }
        else
        {
            error_msg = query.error();
            return EXIT_FAILURE;
        }
    }



    bool mysql_repository::select_by_id(const std::string& key, int id)
    {
        mysqlpp::Query query = conn().query();
        query << "SELECT * FROM `" << key << "` WHERE id=" << id;
        mysqlpp::StoreQueryResult query_result = query.store();
        if (query_result)
        {
            store_names(query_result);
            rows.assign(query_result.begin(), query_result.end());
            return EXIT_SUCCESS;
        }
        else
        {
            error_msg = query.error();
            return EXIT_FAILURE;
        }
    }

    bool mysql_repository::join(const std::string& table_name_A, const std::string& table_name_B)
    {
        mysqlpp::Query query = conn().query();
        query << "SELECT * FROM `" << table_name_A << "` " 
            << "JOIN `" << table_name_B << "` "
            << "ON `" << table_name_A << "`.`id` = `" << table_name_B << "`.`" << table_name_A << "_id`";
        mysqlpp::StoreQueryResult query_result = query.store();
        if (query_result)
        {
            store_names(query_result);
            rows.assign(query_result.begin(), query_result.end());
            return EXIT_SUCCESS;
        }
        else
        {
            error_msg = query.error();
            return EXIT_FAILURE;
        }
    }

    bool mysql_repository::order(const std::string& table_name, const std::string& column, const std::string& order)
    {
        mysqlpp::Query query = conn().query();
        query << "SELECT * FROM `" << table_name + "` "
            << "ORDER BY `" << column << "` " << order;
        mysqlpp::StoreQueryResult query_result = query.store();
        if (query_result)
        {
            store_names(query_result);
            rows.assign(query_result.begin(), query_result.end());
            return EXIT_SUCCESS;
        }
        else
        {
            error_msg = query.error();
            return EXIT_FAILURE;
        }
    }

    //U
    bool mysql_repository::update(const std::string& table_name, const std::string& fields_string, int id)
    {
        mysqlpp::Query query = conn().query(); 
        query << "UPDATE `" << table_name << "` SET " << fields_string << " WHERE `id` = " << id;
        simple_result = query.execute();
        if (simple_result)
        {
            return EXIT_SUCCESS;
        }
        else
        {
            error_msg = query.error();
            return EXIT_FAILURE;
        } 
    }

    //D
bool mysql_repository::delete__(const std::string& table_name, int id)
    {
        mysqlpp::Query query = conn().query();
        query << "DELETE FROM `" << table_name << "` WHERE `id` =" << id;
        simple_result = query.execute();
        if (simple_result)
        {
            return EXIT_SUCCESS;
        }
        else
        {
            error_msg = query.error();
            return EXIT_FAILURE;
        } 
    }
    // getter 
    const char* mysql_repository::error()
    {
        return error_msg;
    }

    const std::vector<mysqlpp::Row>& mysql_repository::get_rows()
    {
        return rows;
    }
    const std::vector<std::string>& mysql_repository::get_names()
    {
        return names;
    }
