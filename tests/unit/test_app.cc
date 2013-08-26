#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "everything.h"

//  http://code.google.com/p/googletest/wiki/Primer#String_Comparison
//  http://code.google.com/p/googletest/wiki/AdvancedGuide#Explicit_Success_and_Failure

class App_Fixture : public ::testing::Test {
    virtual void SetUp() {
        ::theApp = new App({ (char*)"egal", (char*)""} );
    }
};

TEST_F(App_Fixture, greeter) {
    EXPECT_STREQ("Hello, World!", ::theApp->getGreeter());
}

