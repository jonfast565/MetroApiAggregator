#include "Logger.h"

Logger::Logger() {
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    std::string log_name = "AggregatorLog" + this->format_date_file(now) + ".txt";
    this->log_file.open(log_name.c_str(), std::ios::out | std::ios::app | std::ios::binary);
}

std::string Logger::format_date_file(boost::posix_time::ptime now) const {
    std::locale loc(std::cout.getloc(), new boost::posix_time::time_facet("%Y%m%d"));
    std::stringstream s;
    s.imbue(loc);
    s << now;
    return s.str();
}

std::string Logger::format_date_time(boost::posix_time::ptime now) const {
    std::locale loc(std::cout.getloc(), new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S"));
    std::stringstream s;
    s.imbue(loc);
    s << now;
    return s.str();
}

void Logger::write(std::string str, Logger::LogType type) {
    // write log records with the local time
    std::lock_guard<std::mutex> lock(this->io_lock);
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    if (this->log_file.is_open()) {
        switch (type) {
            case LOG_DEFAULT:
                this->log_file << "[ " << "Message @ " << this->format_date_time(now) << " : " << str << " ]" << std::endl;
                break;
            case LOG_ERROR:
                this->log_file << "[ " << "Error   @ " << this->format_date_time(now) << " : " << str << " ]" << std::endl;
                break;
            case LOG_WARNING:
                this->log_file << "[ " << "Warning @ " << this->format_date_time(now) << " : " << str << " ]" << std::endl;
                break;
            case LOG_MESSAGE:
                this->log_file << "[ " << "Message @ " << this->format_date_time(now) << " : " << str << " ]" << std::endl;
                break;
            case LOG_STATUS:
                this->log_file << "[ " << "Status  @ " << this->format_date_time(now) << " : " << str << " ]" << std::endl;
                break;
            default:
                this->log_file << "[        @ " << this->format_date_time(now) << ": " << str << " ]" << std::endl;
                break;
        }
    }
}

void Logger::write(const char* str, Logger::LogType type) {
    this->write(std::string(str), type);
}

Logger::~Logger() {
    this->log_file.close();
}

