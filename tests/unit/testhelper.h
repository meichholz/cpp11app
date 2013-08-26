#ifndef TESTHELPER_H
#define TESTHELPER_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "everything.h"

#define CS(s) const_cast<char*>(s)

//  http://code.google.com/p/googletest/wiki/Primer#String_Comparison
//  http://code.google.com/p/googletest/wiki/AdvancedGuide#Explicit_Success_and_Failure

class AppFixtureBase : public ::testing::Test {
    virtual void SetUp() { ::theApp = new App({ CS("egal"), CS("") } );
    }
    virtual void TearDown() { delete ::theApp; ::theApp = nullptr; }
};

#endif
