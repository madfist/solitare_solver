#include "Log.hpp"

Log::LogLevel Log::log_level = Log::WARN;

Log::Log(LogLevel l) : msg_level(l), msg_written(false) {
}

Log::~Log() {
    if (msg_written)
        std::cout << std::endl;
}

void Log::set_level(LogLevel l) {
    log_level = l;
}

Log& Log::operator<<(std::ostream& (*manip)(std::ostream&)) {
    return write_log(manip);
}