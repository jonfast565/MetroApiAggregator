/* 
 * File:   Logger.h
 * Author: jonathan.fast
 *
 * Created on March 27, 2015, 11:22 AM
 */

#ifndef LOGGER_H
#define	LOGGER_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <fstream>
#include <mutex>

class Logger {
public:
    typedef enum LogTypeVar {
        LOG_DEFAULT,
        LOG_ERROR,
        LOG_WARNING,
        LOG_MESSAGE,
        LOG_STATUS
    } LogType;
    Logger();
    virtual ~Logger();
    void write(std::string str, Logger::LogType type);
    void write(const char* str, Logger::LogType type);
private:
    std::string format_date_file(boost::posix_time::ptime now) const;
    std::string format_date_time(boost::posix_time::ptime now) const;
    std::ofstream log_file;
    std::mutex io_lock;
};

#endif	/* LOGGER_H */

