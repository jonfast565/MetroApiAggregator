#ifndef DATABASE_H
#define	DATABASE_H

// std c++ includes
#include <string>
#include <memory>
#include <chrono>

// MongoDB includes
#include <mongoc.h>
#include <bson.h>

// program includes
#include "Logger.h"
#include "Utility.h"

// vars
static const char* default_mongo = "mongodb://localhost:27017";

// BSON Object
class BSONObject {
private:
    bson_t* internal_object;
    bson_oid_t internal_oid;
public:
    BSONObject();
    virtual ~BSONObject();
    bson_t* get_internal();
    void append_oid_default();
    void append_string(std::string key, std::string value);
    void append_string(const char* key, std::string value);
    void append_string(const char* key, const char* value);
    void append_string_array(const std::string& array_name,
        const std::vector<std::string>& string_array);
    void append_current_datetime(std::string key);
    void append_datetime(std::string key, 
        std::chrono::time_point<std::chrono::system_clock> time);
};

class Database {
private:
    mongoc_client_t* client;
    mongoc_database_t* database;
    mongoc_collection_t* collection;
    bool client_initialized = false;
    bool database_initialized = false;
    bool collection_initialized = false;
    std::string address;
    std::string database_name;
    std::string collection_name;
    std::shared_ptr<Logger> logger;
    void initialize_database(std::string database_name);
    void initialize_collection(std::string collection_name);
    void deinitialize_database();
    void deinitialize_collection();
public:
    Database(std::string const& address, const std::shared_ptr<Logger>& logger);
    Database(const std::shared_ptr<Logger>& logger);
    virtual ~Database();
    void create_get_database(std::string database_name);
    void create_get_collection(std::string collection_name);
    void clear_collection();
    void insert_record(bson_t* bson_record);
    void insert_records(std::shared_ptr<std::vector<std::shared_ptr<BSONObject>>> bson_records);
};

#endif	/* DATABASE_H */

