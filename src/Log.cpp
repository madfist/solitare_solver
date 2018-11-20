#include "Log.hpp"

Log::LogLevel Log::log_level = Log::WARN;
std::ostream* Log::out = &std::cout;

Log::Log(LogLevel l) : msg_level(l), msg_written(false) {
}

Log::~Log() {
    if (msg_written)
        *out << std::endl;
}

void Log::set_level(LogLevel l) {
    log_level = l;
}

void Log::set_output_stream(std::ostream* os) {
    out = os;
}

Log& Log::operator<<(std::ostream& (*manip)(std::ostream&)) {
    return write_log(manip);
}

// PrettyPrintWrapper operator<<(Log& log, PrettyPrint pp) {
//     return PrettyPrintWrapper(*Log::out);
// }