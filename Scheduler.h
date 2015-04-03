/* 
 * File:   Scheduler.h
 * Author: jonathan.fast
 *
 * Created on April 1, 2015, 11:58 AM
 */

#ifndef SCHEDULER_H
#define	SCHEDULER_H

#include <vector>
#include <functional>
#include <thread>
#include <future>
#include <chrono>

#include "Logger.h"

class TaskResult {
    public:
    std::chrono::time_point<std::chrono::system_clock> finished_at;
    uint64_t result_code;
    uint64_t schedule_id;
};

class Schedule {
    public:
    Schedule() {
        this->last_run = std::chrono::system_clock::now() - std::chrono::hours(24);
    }
    uint64_t schedule_id;
    std::chrono::time_point<std::chrono::system_clock> last_run; 
    std::chrono::duration<double> run_every;
    std::string task_name;
    std::function<std::shared_ptr<TaskResult>(uint64_t, std::shared_ptr<Logger>)> runner;
};

class Scheduler {
public:
    Scheduler(std::vector<std::shared_ptr<Schedule>>& schedules, std::shared_ptr<Logger> logger);
    virtual ~Scheduler();
    void run(bool sleep = true);
private:
    std::vector<std::shared_ptr<Schedule>> schedules; 
    std::vector<std::shared_ptr<std::promise<std::shared_ptr<TaskResult>>>> promises;
    std::shared_ptr<Logger> logger;
};

#endif	/* SCHEDULER_H */

