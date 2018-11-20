#ifndef SOLITARE_SOLVER_LOG_HEADER
#define SOLITARE_SOLVER_LOG_HEADER

#include <iostream>
#include "PrettyPrint.hpp"

class Log {
public:
    enum LogLevel {
        DEBUG, INFO, WARN, ERROR
    };

    Log(LogLevel l = WARN);
    ~Log();

    static void set_level(LogLevel l);
    static void set_output_stream(std::ostream*);

    template <class T>
    Log& operator<<(const T& value) {
        return write_log(value);
    }

    Log& operator<<(std::ostream& (*manip)(std::ostream&));

    // friend PrettyPrintWrapper operator<<(Log&, PrettyPrint);
private:
    template <class T>
    Log& write_log(const T& value) {
        if (msg_level >= log_level) {
            *out << value;
            msg_written = true;
        }
        return *this;
    }
    static LogLevel log_level;
    static std::ostream* out;
    LogLevel msg_level;
    bool msg_written;
};

#endif