#ifndef __JSON_UTILS_HEADER__
#define __JSON_UTILS_HEADER__

#include <crow.h>
#include <string.h>

namespace sql_utils
{
    std::string create_key_string(const crow::json::rvalue& data);
    std::string create_value_row_string(const crow::json::rvalue& row);
    std::string create_values_string(const crow::json::rvalue& data);
    std::string crow_json_converter(const crow::json::rvalue& val);
}


#endif
