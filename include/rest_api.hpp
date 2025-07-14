#ifndef __REST_API_
#define __REST_API_

#include <mysql++/mysql++.h>
#include <filesystem>
#include <crow.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <string>


void crow_get_all_entity(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void crow_get_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql);

void crow_create_entity(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void crow_update_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void crow_delete_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void crow_get_joined_entities(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void crow_get_ordered_entities(crow::SimpleApp& app, mysqlpp::Connection& mysql);

#endif
