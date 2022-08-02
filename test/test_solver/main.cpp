#include <string>

#include "gtest/gtest.h"
#include "Trace.hpp"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    if (argc > 1 && std::string(argv[1]) == "--trace") {
        Trace::enable_component(TraceComponent::TEST);
    }
    return RUN_ALL_TESTS();
}
