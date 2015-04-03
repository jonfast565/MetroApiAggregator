/* 
 * File:   Main.h
 * Author: jonathan.fast
 *
 * Created on April 3, 2015, 12:37 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <memory>

#include "Scheduler.h"

int main(int argc, char** argv);
std::shared_ptr<Schedule> make_lines_schedule();
std::shared_ptr<Schedule> make_stations_schedule();
std::shared_ptr<Schedule> make_station_parking_schedule();
std::shared_ptr<Schedule> make_predictions_schedule();
std::shared_ptr<Schedule> make_incidents_schedule();
std::shared_ptr<Schedule> make_elevator_incidents_schedule();

#endif	/* MAIN_H */

