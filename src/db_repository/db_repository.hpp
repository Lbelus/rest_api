#ifndef DB_REPOSITORY_HPP_
#define DB_REPOSITORY_HPP_

#include <mysql++/mysql++.h>
#include <string>

// Client-Side Validation: Perform basic checks (e.g., required fields, formats) to improve user experience.
// Server-Side Validation: Always validate inputs on the server to prevent malicious data from bypassing client-side checks.

class mysql_repository 
{
    private:
        mysqlpp::Connection         conn;
        mysqlpp::SimpleResult       simple_result;
        const char*                 error_msg; 
        std::vector<mysqlpp::Row>   rows;
        std::vector<std::string>    names;
        
        void store_names(const mysqlpp::StoreQueryResult& query_result);
    public:
    // Immediate syntax validation of input data at initilialization level. 
    // flag things that are definitely wrong; e.g. missing required fields, type mismatch, unparsable strings, any attempts of code injection and the presence (or lack of) of security tokens.
        mysql_repository(mysqlpp::Connection conn): conn(conn)
        {
            
            std::cout << "PLACEHOLDER SQL: " << "Server-side validation phase 1" << std::endl;
        }

    // Business-logic validation at method level
    // When this validation passes, the input data is at least syntactically correct and may be passed on to the services, where a more strict validation occurs; i.e. does the input data make sense business-wise, does the resource with that ID exist - and so on.
        //C
        bool insert(const std::string& table_name,const std::string& keys, const std::string& values);
        //R
        bool select_all(const std::string& key);
        bool select_by_id(const std::string& key, int id);
        bool join(const std::string& table_name_A, const std::string& table_name_B);
        bool order(const std::string& table_name, const std::string& column, const std::string& order);
        //U
        bool update(const std::string& table_name, const std::string& fields_string, int id);
        //D
        bool delete__(const std::string& table_name, int id);
        // getter
        const char* error();
        const std::vector<mysqlpp::Row>& get_rows();
        const std::vector<std::string>& get_names();
};

#endif
