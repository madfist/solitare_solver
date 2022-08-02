#ifndef SOLITARE_SOLVER_TRACE_HEADER
#define SOLITARE_SOLVER_TRACE_HEADER

#include <array>
#include <iostream>
#include <string>

#include "VersatileMask.hpp"

enum TraceComponent {
    SOLVER, GAME, TEST
};

class TraceComponentMask : public VersatileMask<uint8_t, TraceComponent> {
public:
    TraceComponentMask();
    bool enabled(TraceComponent) const;
    void enable(TraceComponent);
private:
    uint8_t mask(TraceComponent) const;
    uint8_t shift(TraceComponent) const;

    static const std::array<uint8_t, 3> MASKS;
    static const std::array<uint8_t, 3> SHIFTS;
};

/**
 * @brief Tracer for debugging
 * @details Usage:
 * ```
 * Trace::enable_component(Trace::SOLVER)
 * Trace(TraceComponent::SOLVER) << "message"
 */
class Trace {
public:
    Trace(TraceComponent);
    ~Trace();

    static void enable_component(TraceComponent);
    static void set_output_stream(std::ostream*); ///< Change output stream (default is `std::cout`)

    /// Output stream operator
    template <class T>
    Trace& operator<<(const T& value) {
        return write_log(value);
    }

    /// Output stream operator accepting IO manipulation options
    Trace& operator<<(std::ostream& (*manip)(std::ostream&));
private:
    template <class T>
    Trace& write_log(const T& value) {
        if (enabled_components.enabled(component)) {
            *out << value;
        }
        return *this;
    }
    static TraceComponentMask enabled_components;
    static std::ostream* out;
    static std::string component_strings[];
    TraceComponent component;
    bool msg_written;
};

#endif
