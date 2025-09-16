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

#include <mysql_conn_pool.hpp>

typedef void (*mysql_func_ptr_t) (crow::SimpleApp&, mysqlpp::Connection&);

void simple_crow_get_all_entity(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void simple_crow_get_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void simple_crow_insert_entity(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void simple_crow_update_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void simple_crow_delete_entity_by_id(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void simple_crow_get_joined_entities(crow::SimpleApp& app, mysqlpp::Connection& mysql);
void simple_crow_get_ordered_entities(crow::SimpleApp& app, mysqlpp::Connection& mysql);

// redis routes:

//post
void simple_crow_set(crow::SimpleApp& app, sw::redis::Redis& redis);
void simple_crow_lpush(crow::SimpleApp& app, sw::redis::Redis& redis);
void simple_crow_rpush(crow::SimpleApp& app, sw::redis::Redis& redis);
void simple_crow_hmget(crow::SimpleApp& app, sw::redis::Redis& redis);

// get
void simple_crow_key(crow::SimpleApp& app, sw::redis::Redis& redis);
void simple_crow_get(crow::SimpleApp& app, sw::redis::Redis& redis);
void simple_crow_lpop(crow::SimpleApp& app, sw::redis::Redis& redis);
void simple_crow_rpop(crow::SimpleApp& app, sw::redis::Redis& redis);
void simple_crow_llen(crow::SimpleApp& app, sw::redis::Redis& redis);
void simple_crow_hmset(crow::SimpleApp& app, sw::redis::Redis& redis);
void simple_crow_ping(crow::SimpleApp& app, sw::redis::Redis& redis);
void simple_crow_echo(crow::SimpleApp& app, sw::redis::Redis& redis);
void simple_crow_flushall(crow::SimpleApp& app, sw::redis::Redis& redis);
void simple_crow_info(crow::SimpleApp& app, sw::redis::Redis& redis);


#endif
