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

typedef enum { of_default=0, of_decimal=0, of_hexadecimal, of_binary, of_float } OptionFlags;

class OptionParser {
    public:
             OptionParser() {};
    virtual ~OptionParser() {};
    virtual void on (char o_short, char const *o_long,
            bool &value_out, bool def_value,
            char const *description=nullptr, OptionFlags of=of_default);
    virtual void on (char o_short, char const *o_long,
            unsigned int &value_out, unsigned int def_value,
            char const *description=nullptr, OptionFlags of=of_default);
};

void OptionParser::on (char o_short, char const *o_long,
       bool &value_out, bool def_value, char const *description, OptionFlags of)
{
    cout << "registering " << o_long << " as bool." << endl;
    // TODO: push a new option record
}

void OptionParser::on (char o_short, char const *o_long,
       unsigned int &value_out, unsigned int def_value, char const *description, OptionFlags of)
{
    cout << "registering " << o_long << " as unsigned int." << endl;
    // TODO: push a new option record
}

TEST_F(App_Fixture, options) {
    OptionParser op;
    unsigned int dmask;
    bool verbose;
    // TODO: pack value and default into typed pair instance
    op.on ('v', "verbose", verbose, false, "verbose output");
    op.on ('d', "debug", dmask, 0u, "debugging mask");
    FAIL();
}


