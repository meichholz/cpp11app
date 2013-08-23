#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "everything.h"

/*  http://code.google.com/p/googletest/wiki/Primer#String_Comparison */

class Main_Tester : public ::testing::Test {
    virtual void SetUp() {
        ::theApp = new App({ (char*)"egal", (char*)""} );
    }
};

TEST_F(Main_Tester, sanity) {
  ASSERT_THROW(
            throw (std::runtime_error ("test exception")),
            std::runtime_error );
}

#ifndef USE_GTEST_MAIN
int main(int argc, char **argv) /* or just link -lgtest_main */
{
  ::testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
#endif
