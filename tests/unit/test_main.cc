#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "everything.h"

/*  http://code.google.com/p/googletest/wiki/Primer#String_Comparison */

App *theApp=(App*)0;

class AppTest : public ::testing::Test {
    virtual void SetUp() {
        ::theApp = new App({ (char*)"egal", (char*)""} );
    }
};

TEST_F(AppTest, Dummy) {
  EXPECT_STREQ("Hello, World!", ::theApp->getGreeter());
  ASSERT_THROW(
            throw (std::runtime_error ("test exception")),
            std::runtime_error );
}

int main(int argc, char **argv) /* or just link -lgtest_main */
{
  ::testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
