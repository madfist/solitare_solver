#ifndef PRETTY_PRINT_HEADER
#define PRETTY_PRINT_HEADER

#include <ostream>

template<class Output>
class PrettyPrintWrapper {
public:
    PrettyPrintWrapper(Output& os) : output(os) {}
    Output& output;
};

struct PrettyPrint {};

template<class Output>
PrettyPrintWrapper<Output> operator<<(Output& os, PrettyPrint) { return PrettyPrintWrapper<Output>(os); }

#endif