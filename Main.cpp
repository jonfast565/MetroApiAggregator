// standard includes
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <chrono>
#include <thread>
#include <functional>

// program includes
#include "Logger.h"
#include "HTTPRequestBuilder.h"
#include "Connector.h"
#include "MetroObjects.h"
#include "Database.h"
#include "Scheduler.h"
#include "Main.h"

// main loop
int main(int argc, char** argv) {
    // debug stuff
    bool debug = false;
    if (argc >= 2) {
        if (strcmp(argv[1], "--debug") == 0) {
            debug = true;
        }
    }

    // initialize a logger
    std::shared_ptr<Logger> logger =
            std::shared_ptr<Logger>(new Logger());
    logger->write("Metro API Aggregator started.",
            Logger::LogType::LOG_STATUS);

    // create a list of schedules
    std::vector<std::shared_ptr < Schedule>> schedules;
    schedules.push_back(make_lines_schedule());
    schedules.push_back(make_stations_schedule());
    schedules.push_back(make_station_parking_schedule());
    schedules.push_back(make_predictions_schedule());
    schedules.push_back(make_incidents_schedule());
    schedules.push_back(make_elevator_incidents_schedule());

    // create a scheduler
    std::unique_ptr<Scheduler> scheduler =
            std::unique_ptr<Scheduler>(new Scheduler(schedules, logger));
    
    // run schedule continuously
    while (true) {
        scheduler->run();
    }

    // done
    logger->write("Done!", Logger::LogType::LOG_STATUS);
    logger.reset();
    return 0;
}

std::shared_ptr<Schedule> make_lines_schedule() {
    std::shared_ptr<Schedule> lines_schedule =
            std::shared_ptr<Schedule>(new Schedule());
    lines_schedule->schedule_id = 1;
    lines_schedule->task_name = "Metro Lines";
    lines_schedule->run_every = std::chrono::hours(23);
    lines_schedule->runner = [](uint64_t schedule_id,
            std::shared_ptr<Logger> logger) -> std::shared_ptr<TaskResult> {
                // initialize a database
                std::unique_ptr<Database> database =
                        std::unique_ptr<Database>(new Database(logger));
                // create the database
                database->create_get_database("MetroData");
                std::shared_ptr<MetroDataParser < MetroRailLine>> lines =
                        std::shared_ptr<MetroDataParser < MetroRailLine >> (
                        new MetroDataParser<MetroRailLine>(logger));
                database->create_get_collection("RailLines");
                database->clear_collection();
                database->insert_records(lines->get_bson_records());
                lines.reset();
                database.reset();
                std::shared_ptr<TaskResult> t =
                        std::shared_ptr<TaskResult>(new TaskResult());
                t->finished_at = std::chrono::system_clock::now();
                t->schedule_id = schedule_id;
                t->result_code = 0;
                return std::move(t);
            };
    return lines_schedule;
}

std::shared_ptr<Schedule> make_stations_schedule() {
    // stations
    // initialize a database
    std::shared_ptr<Schedule> stations_schedule =
            std::shared_ptr<Schedule>(new Schedule());
    stations_schedule->schedule_id = 2;
    stations_schedule->task_name = "Metro Stations";
    stations_schedule->run_every = std::chrono::hours(23);
    stations_schedule->runner = [](uint64_t schedule_id,
            std::shared_ptr<Logger> logger) -> std::shared_ptr<TaskResult> {
                std::shared_ptr<Database> database =
                        std::shared_ptr<Database>(new Database(logger));
                // create the database
                database->create_get_database("MetroData");
                std::shared_ptr<MetroDataParser < MetroRailStation>> stations =
                        std::shared_ptr<MetroDataParser < MetroRailStation >> (
                        new MetroDataParser<MetroRailStation>(logger));
                database->create_get_collection("RailStations");
                database->clear_collection();
                database->insert_records(stations->get_bson_records());
                stations.reset();
                database.reset();
                std::shared_ptr<TaskResult> t =
                        std::shared_ptr<TaskResult>(new TaskResult());
                t->finished_at = std::chrono::system_clock::now();
                t->schedule_id = schedule_id;
                t->result_code = 0;
                return t;
            };
    return stations_schedule;
}

std::shared_ptr<Schedule> make_station_parking_schedule() {
    std::shared_ptr<Schedule> parking_schedule =
            std::shared_ptr<Schedule>(new Schedule());
    parking_schedule->schedule_id = 3;
    parking_schedule->task_name = "Metro Station Parking";
    parking_schedule->run_every = std::chrono::hours(23);
    parking_schedule->runner = [](uint64_t schedule_id,
            std::shared_ptr<Logger> logger) -> std::shared_ptr<TaskResult> {
                // station parking
                // initialize a database
                std::shared_ptr<Database> database =
                        std::shared_ptr<Database>(new Database(logger));
                // create the database
                database->create_get_database("MetroData");
                std::shared_ptr<MetroDataParser < MetroRailStationParking>> station_parking =
                        std::shared_ptr<MetroDataParser < MetroRailStationParking >> (
                        new MetroDataParser<MetroRailStationParking>(logger));
                database->create_get_collection("RailStationParking");
                database->clear_collection();
                database->insert_records(station_parking->get_bson_records());
                station_parking.reset();
                database.reset();
                std::shared_ptr<TaskResult> t =
                        std::shared_ptr<TaskResult>(new TaskResult());
                t->finished_at = std::chrono::system_clock::now();
                t->schedule_id = schedule_id;
                t->result_code = 0;
                return t;
            };
    return parking_schedule;
}

std::shared_ptr<Schedule> make_predictions_schedule() {
    std::shared_ptr<Schedule> predictions_schedule =
            std::shared_ptr<Schedule>(new Schedule());
    predictions_schedule->schedule_id = 4;
    predictions_schedule->task_name = "Metro Predictions";
    predictions_schedule->run_every = std::chrono::seconds(20);
    predictions_schedule->runner = [](uint64_t schedule_id,
            std::shared_ptr<Logger> logger) -> std::shared_ptr<TaskResult> {
                // initialize a database
                std::shared_ptr<Database> database =
                        std::shared_ptr<Database>(new Database(logger));
                // create the database
                database->create_get_database("MetroData");
                // get predictions, keep historical data
                std::shared_ptr<MetroDataParser < MetroRailPrediction>> predictions =
                        std::shared_ptr<MetroDataParser < MetroRailPrediction >> (
                        new MetroDataParser<MetroRailPrediction>(logger));
                database->create_get_collection("RailPredictions");
                database->clear_collection();
                std::shared_ptr<std::vector<std::shared_ptr < BSONObject>>>
                        predictions_list = predictions->get_bson_records();
                database->insert_records(predictions_list);
                
                // create historical predictions
                database->create_get_collection("RailPredictionsHistorical");
                database->insert_records(predictions_list);
                
                predictions_list->clear();
                predictions_list.reset();
                predictions.reset();
                database.reset();
                std::shared_ptr<TaskResult> t =
                        std::shared_ptr<TaskResult>(new TaskResult());
                t->finished_at = std::chrono::system_clock::now();
                t->schedule_id = schedule_id;
                t->result_code = 0;
                return t;
            };
    return predictions_schedule;
}

std::shared_ptr<Schedule> make_incidents_schedule() {
    std::shared_ptr<Schedule> incidents_schedule =
            std::shared_ptr<Schedule>(new Schedule());
    incidents_schedule->schedule_id = 5;
    incidents_schedule->task_name = "Metro Incidents";
    incidents_schedule->run_every = std::chrono::seconds(20);
    incidents_schedule->runner = [](uint64_t schedule_id,
            std::shared_ptr<Logger> logger) -> std::shared_ptr<TaskResult> {
                // initialize a database
                std::shared_ptr<Database> database =
                        std::shared_ptr<Database>(new Database(logger));
                // create the database
                database->create_get_database("MetroData");
                // incidents
                std::shared_ptr<MetroDataParser < MetroRailIncident>> incidents =
                        std::shared_ptr<MetroDataParser < MetroRailIncident >> (
                        new MetroDataParser<MetroRailIncident>(logger));
                database->create_get_collection("RailIncidents");
                database->clear_collection();
                std::shared_ptr<std::vector<std::shared_ptr < BSONObject>>> incidents_list =
                        incidents->get_bson_records();
                database->insert_records(incidents_list);
                incidents_list->clear();
                incidents_list.reset();
                incidents.reset();
                database.reset();
                std::shared_ptr<TaskResult> t =
                        std::shared_ptr<TaskResult>(new TaskResult());
                t->finished_at = std::chrono::system_clock::now();
                t->schedule_id = schedule_id;
                t->result_code = 0;
                return t;
            };
    return incidents_schedule;
}

std::shared_ptr<Schedule> make_elevator_incidents_schedule() {
    std::shared_ptr<Schedule> elevator_schedule =
            std::shared_ptr<Schedule>(new Schedule());
    elevator_schedule->schedule_id = 6;
    elevator_schedule->task_name = "Metro Elevator Incidents";
    elevator_schedule->run_every = std::chrono::seconds(20);
    elevator_schedule->runner = [](uint64_t schedule_id,
            std::shared_ptr<Logger> logger) -> std::shared_ptr<TaskResult> {
                // elevator incidents
                // initialize a database
                std::shared_ptr<Database> database =
                        std::shared_ptr<Database>(new Database(logger));
                // create the database
                database->create_get_database("MetroData");
                std::shared_ptr<MetroDataParser < MetroRailElevatorIncident>> elevator_incidents =
                        std::shared_ptr<MetroDataParser < MetroRailElevatorIncident >> (
                        new MetroDataParser<MetroRailElevatorIncident>(logger));
                database->create_get_collection("RailElevatorIncidents");
                database->clear_collection();
                std::shared_ptr<std::vector<std::shared_ptr < BSONObject>>> elevator_incidents_list =
                        elevator_incidents->get_bson_records();
                database->insert_records(elevator_incidents_list);
                elevator_incidents_list->clear();
                elevator_incidents_list.reset();
                elevator_incidents.reset();
                database.reset();
                std::shared_ptr<TaskResult> t =
                        std::shared_ptr<TaskResult>(new TaskResult());
                t->finished_at = std::chrono::system_clock::now();
                t->schedule_id = schedule_id;
                t->result_code = 0;
                return t;
            };
    return elevator_schedule;
}

