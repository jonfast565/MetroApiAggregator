#ifndef METROOBJECTS_H
#define	METROOBJECTS_H

// std cpp includes
#include <string>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>

// JSONCPP includes
#include <json/json.h>

// MongoDB BSON includes
#include <bson.h>

// std c includes (blah))
#include <stdio.h>

#include "Connector.h"
#include "Utility.h"
#include "Database.h"

// query enum

typedef enum MetroQueryVar {
    METRO_LINES,
    METRO_STATIONS,
    METRO_PARKING,
    METRO_PREDICTIONS,
    METRO_ELEVATORS,
    METRO_INCIDENTS,
    METRO_NONE
} MetroQuery;

// connection information
static const char* METRO_API_KEY = "52a235b551ef4871b35a690559835598";
static const char* METRO_API_HOST = "api.wmata.com";

// referential data
static const char* METRO_RAIL_LINES_QUERY = "/Rail.svc/json/jLines";
static const char* METRO_RAIL_STATIONS_QUERY = "/Rail.svc/json/jStations";
static const char* METRO_RAIL_PARKING_QUERY = "/Rail.svc/json/jStationParking";
// predictions
static const char* METRO_RAIL_PREDICTION_QUERY = "/StationPrediction.svc/json/GetPrediction/All";
// incidents
static const char* METRO_RAIL_INCIDENTS_QUERY = "/Incidents.svc/json/Incidents";
static const char* METRO_RAIL_ELEVATOR_INCIDENTS_QUERY = "/Incidents.svc/json/ElevatorIncidents";

// object classes for what will go into the database

class MetroRailInterface {
public:
    virtual ~MetroRailInterface() = default;
    virtual std::shared_ptr<BSONObject> to_bson_object() = 0;
    virtual void pack_json(Json::Value& response) = 0;
};

class MetroRailLine : public MetroRailInterface {
public:
    std::string LineCode;
    std::string BeginStationCode;
    std::string EndStationCode;
    std::string InternalDestination1;
    std::string InternalDestination2;
    std::string DisplayName;

    void pack_json(Json::Value& response) {
        this->LineCode = response["LineCode"].asString();
        this->BeginStationCode = response["StartStationCode"].asString();
        this->EndStationCode = response["EndStationCode"].asString();
        this->InternalDestination1 = response["InternalDestination1"].asString();
        this->InternalDestination2 = response["InternalDestination2"].asString();
        this->DisplayName = response["DisplayName"].asString();
    }

    std::shared_ptr<BSONObject> to_bson_object() {
        std::shared_ptr<BSONObject> doc = std::shared_ptr<BSONObject>(new BSONObject());
        doc->append_oid_default();
        doc->append_string("ObjectType", std::string("METRO_RAIL_LINE"));
        doc->append_string("LineCode", this->LineCode);
        doc->append_string("StartStationCode", this->BeginStationCode);
        doc->append_string("EndStationCode", this->EndStationCode);
        doc->append_string("InternalDestination1", this->InternalDestination1);
        doc->append_string("InternalDestination2", this->InternalDestination2);
        doc->append_string("DisplayName", this->DisplayName);
        doc->append_current_datetime("LastUpdatedDatetime");
        return std::move(doc);
    }
};

class MetroRailStation : public MetroRailInterface {
public:
    std::string StationCode;
    std::string StationCodeTogether1;
    std::string StationCodeTogether2;
    std::string LineCode1;
    std::string LineCode2;
    std::string LineCode3;
    std::string LineCode4;
    std::string Name;
    std::string Latitude;
    std::string Longitude;
    std::string AddressCity;
    std::string AddressState;
    std::string AddressStreet;
    std::string AddressZipCode;

    void pack_json(Json::Value& response) {
        this->StationCode = response["Code"].asString();
        this->LineCode1 = response["LineCode1"].asString();
        this->LineCode2 = response["LineCode2"].asString();
        this->LineCode3 = response["LineCode3"].asString();
        this->LineCode4 = response["LineCode4"].asString();
        this->Latitude = response["Lat"].asString();
        this->Longitude = response["Lon"].asString();
        this->StationCodeTogether1 = response["StationTogether1"].asString();
        this->StationCodeTogether2 = response["StationTogether2"].asString();
        this->AddressCity = response["Address"]["City"].asString();
        this->AddressState = response["Address"]["State"].asString();
        this->AddressZipCode = response["Address"]["Zip"].asString();
        this->AddressStreet = response["Address"]["Street"].asString();
        this->Name = response["Name"].asString();
    }

    std::shared_ptr<BSONObject> to_bson_object() {
        std::shared_ptr<BSONObject> doc = std::shared_ptr<BSONObject>(new BSONObject());
        doc->append_oid_default();
        doc->append_string("ObjectType", std::string("METRO_RAIL_STATION"));
        doc->append_string("StationCode", this->StationCode);
        doc->append_string("Name", this->Name);
        doc->append_string("LineCode1", this->LineCode1);
        doc->append_string("LineCode2", this->LineCode2);
        doc->append_string("LineCode3", this->LineCode3);
        doc->append_string("LineCode4", this->LineCode4);
        doc->append_string("Latitude", this->Latitude);
        doc->append_string("Longitude", this->Longitude);
        doc->append_string("StationTogether1", this->StationCodeTogether1);
        doc->append_string("StationTogether2", this->StationCodeTogether2);
        doc->append_string("AddressCity", this->AddressCity);
        doc->append_string("AddressState", this->AddressState);
        doc->append_string("AddressZipCode", this->AddressZipCode);
        doc->append_string("AddressStreet", this->AddressStreet);
        doc->append_current_datetime("LastUpdatedDatetime");
        return std::move(doc);
    }
};

class MetroRailStationParking : public MetroRailInterface {
public:
    std::string StationCode;
    std::string Notes;
    std::string AllDayParkingTotalCount;
    std::string AllDayParkingRiderCost;
    std::string AllDayParkingNonRiderCost;
    std::string ShortTermParkingTotalCount;
    std::string ShortTermParkingNotes;

    void pack_json(Json::Value& response) {
        this->StationCode = response["Code"].asString();
        this->Notes = response["Notes"].asString();
        this->AllDayParkingNonRiderCost = response["AllDayParking"]["NonRiderCost"].asString();
        this->AllDayParkingRiderCost = response["AllDayParking"]["RiderCost"].asString();
        this->AllDayParkingTotalCount = response["AllDayParking"]["TotalCount"].asString();
        this->ShortTermParkingTotalCount = response["ShortTermParking"]["TotalCount"].asString();
        this->ShortTermParkingNotes = response["ShortTermParking"]["Notes"].asString();
    }

    std::shared_ptr<BSONObject> to_bson_object() {
        std::shared_ptr<BSONObject> doc = std::shared_ptr<BSONObject>(new BSONObject());
        doc->append_oid_default();
        doc->append_string("ObjectType", std::string("METRO_RAIL_STATION_PARKING"));
        doc->append_string("StationCode", this->StationCode);
        doc->append_string("Notes", this->Notes);
        doc->append_string("AllDayParkingRiderCost", this->AllDayParkingRiderCost);
        doc->append_string("AllDayParkingNonRiderCost", this->AllDayParkingNonRiderCost);
        doc->append_string("AllDayParkingTotalCount", this->AllDayParkingTotalCount);
        doc->append_string("ShortTermParkingTotalCount", this->ShortTermParkingTotalCount);
        doc->append_string("ShortTermParkingNotes", this->ShortTermParkingNotes);
        doc->append_current_datetime("LastUpdatedDatetime");
        return std::move(doc);
    }
};

class MetroRailPrediction : public MetroRailInterface {
public:
    std::string LineCode;
    std::string LocationStationCode;
    std::string DestinationStationCode;
    std::string DestinationDisplayValue;
    std::string CarNumber;
    std::string Group;
    std::string MinutesToArrival;

    void pack_json(Json::Value& response) {
        this->MinutesToArrival = response["Min"].asString();
        this->CarNumber = response["Car"].asString();
        this->LineCode = response["Line"].asString();
        this->Group = response["Group"].asString();
        this->LocationStationCode = response["LocationCode"].asString();
        this->DestinationStationCode = response["DestinationCode"].asString();
        this->DestinationDisplayValue = response["Destination"].asString();
    }

    std::shared_ptr<BSONObject> to_bson_object() {
        std::shared_ptr<BSONObject> doc = std::shared_ptr<BSONObject>(new BSONObject());
        doc->append_oid_default();
        doc->append_string("ObjectType", std::string("METRO_RAIL_PREDICTION"));
        doc->append_string("MinutesToArrival", this->MinutesToArrival);
        doc->append_string("CarNumber", this->CarNumber);
        doc->append_string("LineCode", this->LineCode);
        doc->append_string("Group", this->Group);
        doc->append_string("LocationStationCode", this->LocationStationCode);
        doc->append_string("DestinationStationCode", this->DestinationStationCode);
        doc->append_string("DestinationDisplayValue", this->DestinationDisplayValue);
        doc->append_current_datetime("LastUpdatedDatetime");
        return std::move(doc);
    }
};

class MetroRailIncident : public MetroRailInterface {
public:
    std::string DateUpdated;
    std::string Description;
    std::string IncidentID;
    std::string IncidentType;
    std::unique_ptr<std::vector<std::string>> LineCodesAffected;

    void pack_json(Json::Value& response) {
        this->DateUpdated = response["DateUpdated"].asString();
        this->IncidentID = response["IncidentID"].asString();
        this->IncidentType = response["IncidentType"].asString();
        this->Description = response["Description"].asString();
        this->LineCodesAffected = std::unique_ptr<std::vector < std::string >> (new std::vector<std::string>());
        std::string line_code;
        std::stringstream line_codes(response["LineCodesAffected"].asString());
        while (std::getline(line_codes, line_code, ';')) {
            this->LineCodesAffected->push_back(line_code);
        }
    }

    std::shared_ptr<BSONObject> to_bson_object() {
        std::shared_ptr<BSONObject> doc = std::shared_ptr<BSONObject>(new BSONObject());
        doc->append_oid_default();
        doc->append_string("ObjectType", std::string("METRO_RAIL_INCIDENT"));
        doc->append_string("DateUpdated", this->DateUpdated);
        doc->append_string("Description", this->Description);
        doc->append_string("IncidentID", this->IncidentID);
        doc->append_string("IncidentType", this->IncidentType);
        doc->append_string_array("LineCodesAffected", *this->LineCodesAffected);
        doc->append_current_datetime("LastUpdatedDatetime");
        return std::move(doc);
    }
};

class MetroRailElevatorIncident : public MetroRailInterface {
public:
    std::string StationCode;
    std::string DateUpdated;
    std::string DateOutOfServ;
    std::string LocationDescription;
    std::string SymptomDescription;
    std::string UnitName;
    std::string UnitType;

    void pack_json(Json::Value& response) {
        this->DateUpdated = response["DateUpdated"].asString();
        this->DateOutOfServ = response["DateOutOfServ"].asString();
        this->LocationDescription = response["LocationDescription"].asString();
        this->StationCode = response["StationCode"].asString();
        this->SymptomDescription = response["SymptomDescription"].asString();
        this->UnitName = response["UnitName"].asString();
        this->UnitType = response["UnitType"].asString();
    }

    std::shared_ptr<BSONObject> to_bson_object() {
        std::shared_ptr<BSONObject> doc = std::shared_ptr<BSONObject>(new BSONObject());
        doc->append_oid_default();
        doc->append_string("ObjectType", std::string("METRO_RAIL_ELEVATOR_INCIDENT"));
        doc->append_string("DateUpdated", this->DateUpdated);
        doc->append_string("DateOutOfServ", this->DateOutOfServ);
        doc->append_string("LocationDescription", this->LocationDescription);
        doc->append_string("StationCode", this->StationCode);
        doc->append_string("SymptomDescription", this->SymptomDescription);
        doc->append_string("UnitName", this->UnitName);
        doc->append_string("UnitType", this->UnitType);
        doc->append_current_datetime("LastUpdatedDatetime");
        return std::move(doc);
    }
};

// metro data parser

template <class T>
class MetroDataParser {
private:
    Json::Value root;
    Json::Reader reader;
    MetroQuery query_type;
    std::shared_ptr<Logger> logger;
    std::unordered_map<std::type_index, MetroQuery> type_names;

    std::vector<T*>* get_objects() {
        // legacy
        std::vector<T*>* objects = new std::vector<T*>();
        std::vector<MetroRailInterface*>* returned_objects = this->get_objects_internal();
        std::transform(returned_objects->begin(), returned_objects->end(),
                std::back_inserter(*objects),
                [](MetroRailInterface * t) {
                    return static_cast<T*> (t);
                });
        returned_objects->clear();
        delete returned_objects;
        return objects;
    }

    std::shared_ptr<std::vector<std::shared_ptr<MetroRailInterface>>> get_objects_internal() {
        // uses
        std::shared_ptr<HTTPRequestBuilder> request;
        std::shared_ptr<Connector> connector;
        Json::Value response_root, data;
        Json::Reader response_reader;
        std::string response;
        std::string root_object_key;
        std::string query_string;
        std::string failure_message;

        // returns
        std::shared_ptr<std::vector<std::shared_ptr < MetroRailInterface>>> objects =
                std::shared_ptr<std::vector<std::shared_ptr < MetroRailInterface>>>(
                new std::vector<std::shared_ptr < MetroRailInterface >> ());

        // get the request types and root object keys
        switch (this->query_type) {
            case MetroQuery::METRO_LINES:
                logger->write(std::string("Getting metro rail lines data..."), Logger::LogType::LOG_STATUS);
                root_object_key = "Lines";
                failure_message = "Failed to parse lines JSON";
                query_string = METRO_RAIL_LINES_QUERY;
                break;
            case MetroQuery::METRO_STATIONS:
                logger->write(std::string("Getting metro rail station data..."), Logger::LogType::LOG_STATUS);
                root_object_key = "Stations";
                failure_message = "Failed to parse stations JSON";
                query_string = METRO_RAIL_STATIONS_QUERY;
                break;
            case MetroQuery::METRO_PARKING:
                logger->write(std::string("Getting metro rail station parking data..."), Logger::LogType::LOG_STATUS);
                root_object_key = "StationParking";
                failure_message = "Failed to parse stations parking JSON";
                query_string = METRO_RAIL_PARKING_QUERY;
                break;
            case MetroQuery::METRO_PREDICTIONS:
                logger->write(std::string("Getting metro rail predictions..."), Logger::LogType::LOG_STATUS);
                root_object_key = "Trains";
                failure_message = "Failed to parse predictions JSON";
                query_string = METRO_RAIL_PREDICTION_QUERY;
                break;
            case MetroQuery::METRO_INCIDENTS:
                logger->write(std::string("Getting metro rail incidents data..."), Logger::LogType::LOG_STATUS);
                root_object_key = "Incidents";
                failure_message = "Failed to parse incidents JSON";
                query_string = METRO_RAIL_INCIDENTS_QUERY;
                break;
            case MetroQuery::METRO_ELEVATORS:
                logger->write(std::string("Getting metro rail elevator outages data..."), Logger::LogType::LOG_STATUS);
                root_object_key = "ElevatorIncidents";
                failure_message = "Failed to parse elevator outages JSON";
                query_string = METRO_RAIL_ELEVATOR_INCIDENTS_QUERY;
        }

        // run query
        request = std::shared_ptr<HTTPRequestBuilder>(new HTTPRequestBuilder(METRO_API_HOST, query_string));
        request->add_parameter("api_key", METRO_API_KEY);
        connector = std::shared_ptr<Connector>(new Connector(request, this->logger));
        response = Utility::strip_to_json(connector->run_and_get_response());

        // parse JSON
        if (!response_reader.parse(response, response_root)) {
            this->logger->write(failure_message, Logger::LogType::LOG_ERROR);
        }

        // extract data to objects
        data = response_root[root_object_key.c_str()];
        for (int i = 0; i < data.size(); i++) {
            // create interface object
            std::shared_ptr<MetroRailInterface> mri;
            switch (this->query_type) {
                case MetroQuery::METRO_LINES:
                    mri = std::shared_ptr<MetroRailInterface>(new MetroRailLine());
                    break;
                case MetroQuery::METRO_STATIONS:
                    mri = std::shared_ptr<MetroRailInterface>(new MetroRailStation());
                    break;
                case MetroQuery::METRO_PARKING:
                    mri = std::shared_ptr<MetroRailInterface>(new MetroRailStationParking());
                    break;
                case MetroQuery::METRO_PREDICTIONS:
                    mri = std::shared_ptr<MetroRailInterface>(new MetroRailPrediction());
                    break;
                case MetroQuery::METRO_INCIDENTS:
                    mri = std::shared_ptr<MetroRailInterface>(new MetroRailIncident());
                    break;
                case MetroQuery::METRO_ELEVATORS:
                    mri = std::shared_ptr<MetroRailInterface>(new MetroRailElevatorIncident());
                    break;
                case MetroQuery::METRO_NONE:
                default:
                    logger->write(std::string("Invalid object or option"), Logger::LogType::LOG_ERROR);
                    break;
            }

            // pack 
            mri->pack_json(data[i]);
            objects->push_back(mri);
        }
        return objects;
    }
public:

    MetroDataParser(std::shared_ptr<Logger> logger) {
        this->type_names[std::type_index(typeid (MetroRailLine))] = MetroQuery::METRO_LINES;
        this->type_names[std::type_index(typeid (MetroRailStation))] = MetroQuery::METRO_STATIONS;
        this->type_names[std::type_index(typeid (MetroRailStationParking))] = MetroQuery::METRO_PARKING;
        this->type_names[std::type_index(typeid (MetroRailPrediction))] = MetroQuery::METRO_PREDICTIONS;
        this->type_names[std::type_index(typeid (MetroRailIncident))] = MetroQuery::METRO_INCIDENTS;
        this->type_names[std::type_index(typeid (MetroRailElevatorIncident))] = MetroQuery::METRO_ELEVATORS;
        this->query_type = this->type_names[std::type_index(typeid (T))];
        this->logger = logger;
    }
    virtual ~MetroDataParser() = default;

    std::shared_ptr<std::vector<std::shared_ptr<BSONObject>>> get_bson_records() {
        std::shared_ptr<std::vector<std::shared_ptr < BSONObject>>> records =
                std::unique_ptr<std::vector<std::shared_ptr < BSONObject>>>
                (new std::vector<std::shared_ptr < BSONObject >> ());
        std::shared_ptr<std::vector<std::shared_ptr < MetroRailInterface>>>
                returned_objects = this->get_objects_internal();
        std::transform(returned_objects->begin(), returned_objects->end(),
                std::back_inserter(*records),
                [](std::shared_ptr<MetroRailInterface> t) {
                    return t->to_bson_object();
                });
        returned_objects->clear();
        returned_objects.reset();
        return std::move(records);
    }
};

#endif	/* METROOBJECTS_H */

