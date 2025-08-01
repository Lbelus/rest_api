#include <sql_json_utils.hpp>

namespace sql_utils
{
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
