#include <sstream>

#include "gtest/gtest.h"
#include "Log.hpp"

TEST(test_log, level) {
    std::stringstream ss;
    Log::set_level(Log::ERROR);
    Log::set_output_stream(&ss);

    Log() << "Default warning message, stream should be empty";
    EXPECT_EQ(ss.str(), std::string());

    Log(Log::DEBUG) << "Debug message, stream should be empty";
    EXPECT_EQ(ss.str(), std::string());

    std::string error_message = "Error message should appear";
    Log(Log::ERROR) << error_message;
    EXPECT_EQ(ss.str(), error_message + '\n');
}
