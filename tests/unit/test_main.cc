#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "app.h"

/*  http://code.google.com/p/googletest/wiki/Primer#String_Comparison */

App *theApp=(App*)0;

static char *args1[]={ (char*)"egal", (char*)""};

class AppTest : public ::testing::Test {
    virtual void SetUp() {
        ::theApp = new App(0, ::args1);
    }
};

TEST_F(AppTest, Dummy) {
  EXPECT_STREQ("Hello, World!", ::theApp->getGreeter());
}

int main(int argc, char **argv) /* or just link -lgtest_main */
{
  ::testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
