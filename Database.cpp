#include "Database.h"

/* BSON wrapper for creating BSON data using Mongoc */
BSONObject::BSONObject() {
    this->internal_object = bson_new();
}

BSONObject::~BSONObject() {
    if (this->internal_object != NULL) {
        bson_destroy(this->internal_object);
    }
}

bson_t* BSONObject::get_internal() {
    return this->internal_object;
}

void BSONObject::append_oid_default() {
    bson_oid_init(&this->internal_oid, NULL);
    bson_append_oid(this->internal_object, "_id", -1, &this->internal_oid);
}

void BSONObject::append_string(std::string key, std::string value) {
    bson_append_utf8(this->internal_object, key.c_str(), -1, value.c_str(), -1);
}

void BSONObject::append_string(const char* key, const char* value) {
    this->append_string(std::string(key), std::string(value));
}

void BSONObject::append_string(const char* key, std::string value) {
    this->append_string(std::string(key), value);
}

void BSONObject::append_string_array(const std::string& array_name,
        const std::vector<std::string>& string_array) {
    int counter = 0;
    bson_t* child_doc = bson_new();
    bson_append_array_begin(this->internal_object, array_name.c_str(), -1, child_doc);
    for (auto it = string_array.begin(); it != string_array.end(); it++) {
        char currentNumber[100];
        sprintf(currentNumber, "%d", counter);
        bson_append_utf8(child_doc, currentNumber, -1, (*it).c_str(), -1);
        counter++;
    }
    bson_append_array_end(this->internal_object, child_doc);
}

void BSONObject::append_current_datetime(std::string key) {
    std::int64_t milliseconds_since_epoch =
            std::chrono::system_clock::now().time_since_epoch() /
            std::chrono::milliseconds(1);
    bson_append_date_time(this->internal_object, key.c_str(), -1, milliseconds_since_epoch);
}

void BSONObject::append_datetime(std::string key, std::chrono::time_point<std::chrono::system_clock> time) {
    std::int64_t milliseconds_since_epoch = time.time_since_epoch() /
            std::chrono::milliseconds(1);
    bson_append_date_time(this->internal_object, key.c_str(), -1, milliseconds_since_epoch);
}

/* Database objects */
Database::Database(std::string const& address, const std::shared_ptr<Logger>& logger) {
    this->address = address;
    this->client = mongoc_client_new(address.c_str());
    this->client_initialized = true;
    this->logger = logger;
    this->logger->write("Database initialized", Logger::LogType::LOG_STATUS);
    this->database_name = "";
    this->collection_name = "";
}

Database::Database(const std::shared_ptr<Logger>& logger) {
    this->client = mongoc_client_new(default_mongo);
    this->client_initialized = true;
    this->logger = logger;
    this->logger->write("Database initialized", Logger::LogType::LOG_STATUS);
}

void Database::initialize_database(std::string database_name) {
    this->database = mongoc_client_get_database(this->client, database_name.c_str());
    this->database_initialized = true;
    this->database_name = database_name;
}

void Database::initialize_collection(std::string collection_name) {
    this->collection = mongoc_client_get_collection(this->client,
            this->database_name.c_str(), collection_name.c_str());
    this->collection_initialized = true;
    this->collection_name = collection_name;
}

void Database::deinitialize_database() {
    if (this->database_initialized == true) {
        mongoc_database_destroy(this->database);
    }
    this->database = NULL;
    this->database_name = "";
    this->database_initialized = false;
}

void Database::deinitialize_collection() {
    if (this->collection_initialized == true) {
        mongoc_collection_destroy(this->collection);
    }
    this->collection = NULL;
    this->collection_name = "";
    this->collection_initialized = false;
}

Database::~Database() {
    this->deinitialize_collection();
    this->deinitialize_database();
    if (this->client_initialized == true) {
        mongoc_client_destroy(this->client);
        this->client_initialized = false;
    }
}

void Database::create_get_database(std::string database_name) {
    if (this->client_initialized == true) {
        this->deinitialize_collection();
        this->deinitialize_database();
        this->initialize_database(database_name);
        this->logger->write(std::string("Accessing database ")
                + this->database_name, Logger::LogType::LOG_STATUS);
    } else {
        throw ApiException("Could not access database "
                + database_name);
    }
}

void Database::create_get_collection(std::string collection_name) {
    if (this->client_initialized == true
            && this->database_initialized == true) {
        this->deinitialize_collection();
        this->initialize_collection(collection_name);
        this->logger->write(std::string("Accessing collection ")
                + this->collection_name, Logger::LogType::LOG_STATUS);
    } else {
        throw ApiException("Could not access collection "
                + collection_name);
    }
}

void Database::clear_collection() {
    bson_t* empty_record = bson_new();
    bson_error_t error;
    if (!mongoc_collection_remove(this->collection,
            MONGOC_REMOVE_NONE, empty_record, NULL, &error)) {
        throw ApiException(std::string("Collection delete failed with error: ")
                + std::string(error.message));
    }
    bson_destroy(empty_record);
}

void Database::insert_record(bson_t* bson_record) {
    if (this->client_initialized == true
            && this->database_initialized == true
            && this->collection_initialized == true) {
        bson_error_t error;
        if (!mongoc_collection_insert(this->collection,
                MONGOC_INSERT_NONE, bson_record, NULL, &error)) {
            throw ApiException(std::string("Insert record failed with error: ")
                    + std::string(error.message));
        }
    } else {
        throw ApiException("Database not in an initialized state upon insert");
    }
}

void Database::insert_records(std::shared_ptr<std::vector<std::shared_ptr<BSONObject>>> bson_records) {
    std::vector<std::shared_ptr < BSONObject>>::iterator it;
    for (it = bson_records->begin(); it != bson_records->end(); it++) {
        this->insert_record((*it)->get_internal());
    }
    logger->write(std::to_string(bson_records->size()) + std::string(" records inserted."),
            Logger::LogType::LOG_STATUS);
}

