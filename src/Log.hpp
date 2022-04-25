#ifndef SOLITARE_SOLVER_LOG_HEADER
#define SOLITARE_SOLVER_LOG_HEADER

#include <iostream>
#include "PrettyPrint.hpp"

/**
 * @brief Logger
 * @details Usage:
 * ```
 * Log::set_level(Log::DEBUG)
 * Log() << "Message on default level"
 * Log(Log::ERROR) << "Message on error level";
 * ```
 */
class Log {
public:
    /// Minimum log level to display messages
    enum LogLevel {
        DEBUG, INFO, WARN, ERROR
    };

    Log(LogLevel l = WARN); ///< Construct a logger instance to stream messages into
    ~Log();                 ///< Destruct logger by adding `std::endl` at the end of the message

    static void set_level(LogLevel l);            ///< Change minimum log level
    static void set_output_stream(std::ostream*); ///< Change output stream (default is `std::cout`)

    /// Output stream operator
    template <class T>
    Log& operator<<(const T& value) {
        return write_log(value);
    }

    /// Output stream operator accepting IO manipulation options
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
