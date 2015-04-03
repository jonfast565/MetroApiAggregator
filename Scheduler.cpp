#include "Scheduler.h"

Scheduler::Scheduler(std::vector<std::shared_ptr<Schedule>>&schedules,
        std::shared_ptr<Logger> logger) {
    this->schedules = schedules;
    this->logger = logger;
}

void Scheduler::run(bool sleep) {
    // get the current time
    std::chrono::time_point<std::chrono::system_clock> start =
            std::chrono::system_clock::now();
    
    // iterate through schedules
    std::vector<std::shared_ptr < Schedule>>::iterator schedule_it;
    for (schedule_it = this->schedules.begin();
            schedule_it != this->schedules.end(); schedule_it++) {
        
        // get the last run time difference
        std::chrono::duration<double> time_since_update = start - (*schedule_it)->last_run;
        
        // if it is time for the update cycle to run
        if (time_since_update >= (*schedule_it)->run_every) {
            
            // create a promise for the task
            std::shared_ptr<std::promise<std::shared_ptr < TaskResult>>> schedule_promise
                    = std::shared_ptr<std::promise<std::shared_ptr < TaskResult>>>
                    (new std::promise<std::shared_ptr < TaskResult >> ());
            this->promises.push_back(schedule_promise);
            
            // run it
            std::thread([](std::shared_ptr<std::promise<std::shared_ptr < TaskResult>>> p,
                    std::function < std::shared_ptr<TaskResult>(uint64_t, std::shared_ptr<Logger>) > res,
                    uint64_t schedule_id, std::shared_ptr<Logger> logger) {
                p->set_value(res(schedule_id, logger));
            }, schedule_promise,
                    (*schedule_it)->runner,
                    (*schedule_it)->schedule_id,
                    this->logger).detach();
        }
    }
    
    // wait for the promises to complete
    std::vector<std::shared_ptr < std::promise<std::shared_ptr < TaskResult>>>>::iterator future_it;
    for (future_it = this->promises.begin();
            future_it != this->promises.end(); future_it++) {
        
        // get each associated future and wait for task completion
        std::future<std::shared_ptr < TaskResult>> schedule_future = (*future_it)->get_future();
        schedule_future.wait();
        std::shared_ptr<TaskResult> result = schedule_future.get();
        
        // set the last run values from the task results
        for (schedule_it = this->schedules.begin();
                schedule_it != this->schedules.end(); schedule_it++) {
            if (result->schedule_id == (*schedule_it)->schedule_id) {
                (*schedule_it)->last_run = result->finished_at;
            }
        }
    }
    
    // remove all old promises
    this->promises.clear();
    
    // sleep if necessary, prevents CPU overload
    if (sleep) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

Scheduler::~Scheduler() {
    this->schedules.clear();
}

