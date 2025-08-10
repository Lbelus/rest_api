#ifndef __REST_API_
#define __REST_API_

#include <mysql++/mysql++.h>
#include <sw/redis++/redis++.h>
#include <filesystem>
#include <crow.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <string>


typedef void (*mysql_func_ptr_t) (crow::SimpleApp&, mysqlpp::Connection&);

void crow_get_all_entity(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void crow_get_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void crow_insert_entity(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void crow_update_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void crow_delete_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void crow_get_joined_entities(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void crow_get_ordered_entities(crow::SimpleApp& app, mysqlpp::Connection& mysql);

// redis routes:

//post
void crow_set(crow::SimpleApp& app, sw::redis::Redis& redis);
void crow_lpush(crow::SimpleApp& app, sw::redis::Redis& redis);
void crow_rpush(crow::SimpleApp& app, sw::redis::Redis& redis);
void crow_hmget(crow::SimpleApp& app, sw::redis::Redis& redis);

// get
void crow_key(crow::SimpleApp& app, sw::redis::Redis& redis);
void crow_get(crow::SimpleApp& app, sw::redis::Redis& redis);
void crow_lpop(crow::SimpleApp& app, sw::redis::Redis& redis);
void crow_rpop(crow::SimpleApp& app, sw::redis::Redis& redis);
void crow_llen(crow::SimpleApp& app, sw::redis::Redis& redis);
void crow_hmset(crow::SimpleApp& app, sw::redis::Redis& redis);
void crow_ping(crow::SimpleApp& app, sw::redis::Redis& redis);
void crow_echo(crow::SimpleApp& app, sw::redis::Redis& redis);
void crow_flushall(crow::SimpleApp& app, sw::redis::Redis& redis);
void crow_info(crow::SimpleApp& app, sw::redis::Redis& redis);


#endif
