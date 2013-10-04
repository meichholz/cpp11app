#include "testhelper.h"


TEST_F(AppFixtureBase, greeter) {
    EXPECT_STREQ("Hello, World!", ::theApp->getGreeter());
}

