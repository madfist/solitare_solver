#include "Trace.hpp"

TraceComponentMask Trace::enabled_components;
std::ostream* Trace::out = &std::cout;
std::string Trace::component_strings[] = {"SOLVER", "GAME", "TEST", "PARALLEL"};

const std::array<uint8_t, 4> TraceComponentMask::MASKS = {0x1, 0x2, 0x4, 0x8};
const std::array<uint8_t, 4> TraceComponentMask::SHIFTS = {0, 1, 2, 3};

TraceComponentMask::TraceComponentMask() : VersatileMask(0) {}

bool TraceComponentMask::enabled(TraceComponent c) const {
    return get(c);
}

void TraceComponentMask::enable(TraceComponent c) {
    set(c, 1);
}

uint8_t TraceComponentMask::mask(TraceComponent m) const {
    return MASKS[m];
}
uint8_t TraceComponentMask::shift(TraceComponent m) const {
    return SHIFTS[m];
}

Trace::Trace(TraceComponent c) : component(c), msg_written(false) {
    if (enabled_components.enabled(component)) {
        *out << "~~~ " << component_strings[component] << " ~~~\n";
        msg_written = true;
    }
}

Trace::~Trace() {
    if (msg_written)
        *out << "\n=== === ===\n";
}

void Trace::enable_component(TraceComponent c) {
    enabled_components.enable(c);
}

void Trace::set_output_stream(std::ostream* os) {
    out = os;
}

Trace& Trace::operator<<(std::ostream& (*manip)(std::ostream&)) {
    return write_log(manip);
}
