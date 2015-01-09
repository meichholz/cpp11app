#include "testhelper.h"

//  http://code.google.com/p/googletest/wiki/Primer#String_Comparison
//  http://code.google.com/p/googletest/wiki/AdvancedGuide#Explicit_Success_and_Failure
// http://www.cplusplus.com/reference/string/string/

using namespace testing; // for google mock and so on

class OptionParserFixture // {{{ have a generic parser with standard options
    : public AppFixtureBase {
    protected:
        Option::Parser *op;
        unsigned int dmask;
        string config_file_name;
        string log_file_name;
        bool verbose;
        const char *version;
        const char *programname;
        virtual void SetUp() {
            AppFixtureBase::SetUp();
            // the generic initialisations will be overridden by defaults, nevertheless
            verbose = true; // will be overridden
            dmask = 1234u;
            config_file_name = "totalegal";
            log_file_name = "nochegaler";
            // test values to DRY matchers
            version = "0.1.2"_cs;
            programname = "program"_cs;
            // setup the parser with various matchers
            op = new Option::Parser(programname, version); // simple bool
            op->on ('v', "verbose", verbose, "verbose output"); // templated function w/default ref and rvalue
            op->on<unsigned int>('d', "debug", dmask, 7u, "debugging mask"); // with Option::Value<T>
            op->on ('c', "config", new Option::Value<string>(&config_file_name, "./x.ini"), "configfile name"); // with string
            op->on ('l', "log", log_file_name, "./x.log", "log file name", Option::Flags::FILENAME); // string as file name
        }
        virtual void TearDown() {
            delete op;
            op = nullptr;
            AppFixtureBase::TearDown();
        }
};
/// }}}
TEST_F(OptionParserFixture, SetsDefaultValues) /* {{{ */ {
    EXPECT_EQ(OK, op->parse(vector<string>({ programname })));
    EXPECT_EQ(verbose, false);
    EXPECT_EQ(dmask, 7u);
    EXPECT_STREQ(config_file_name.c_str(), "./x.ini");
    EXPECT_STREQ(log_file_name.c_str(), "./x.log");
}
/// }}}
TEST_F(OptionParserFixture, SetsUserProvidedValues) /* {{{ */ {
    EXPECT_EQ(OK, op->parse(vector<string>({ programname, "--verbose"_cs, "-d"_cs, "23"_cs, "--config=frob"_cs, "--log=log.log"_cs })));
    EXPECT_EQ(verbose, true);
    EXPECT_EQ(dmask, 23u);
    EXPECT_STREQ(config_file_name.c_str(), "frob");
    EXPECT_STREQ(log_file_name.c_str(), "log.log");
}
/// }}}
TEST_F(OptionParserFixture, AcceptsFileArgument) /* {{{ */ {
    EXPECT_EQ(OK, op->parse(vector<string>({ programname, "-v"_cs, "file_x"_cs })));
    EXPECT_EQ(verbose, true);
}
/// }}}
TEST_F(OptionParserFixture, StreamsUsagePage) { // {{{
    ostringstream os;
    op->showHelp(os);
    EXPECT_THAT(os.str(), HasSubstr("{OPTION}"));
    EXPECT_THAT(os.str(), HasSubstr("-v/--verbose"));
}
/// }}}


