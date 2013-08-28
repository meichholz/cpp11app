#include "testhelper.h"

//  http://code.google.com/p/googletest/wiki/Primer#String_Comparison
//  http://code.google.com/p/googletest/wiki/AdvancedGuide#Explicit_Success_and_Failure
// http://www.cplusplus.com/reference/string/string/

class OptionParserFixture : public AppFixtureBase { // {{{
    protected:
        Option::Parser *op;
        unsigned int dmask;
        string config_file_name;
        bool verbose;
        virtual void SetUp() {
            AppFixtureBase::SetUp();
            verbose = true;
            dmask = 1234u;
            config_file_name = "totalegal";
            op = new Option::Parser("cpp", "nothing");
            op->on ('v', "verbose", &verbose, "verbose output");
            op->on ('c', "config", new Option::Value<string>(&config_file_name, "./x.ini"), "configfile name");
            op->on ('d', "debug", new Option::Value<unsigned int>(&dmask, 7u), "debugging mask");
        }
        virtual void TearDown() {
            delete op;
            op = nullptr;
            AppFixtureBase::TearDown();
        }
};
/// }}}
TEST_F(OptionParserFixture, TestOptionSetter) {
    op->parse(vector<string>({ "egal"_cs }));
    EXPECT_EQ(verbose, false);
    EXPECT_EQ(dmask, 7u);
    EXPECT_STREQ(config_file_name.c_str(), "./x.ini");
    op->parse(vector<string>({ "egal"_cs, "-v"_cs, "-d"_cs, "23"_cs }));
    EXPECT_EQ(verbose, true);
    EXPECT_EQ(dmask, 23u);
    EXPECT_STREQ(config_file_name.c_str(), "./x.ini");
    // TODO: string set implementieren
    // TODO: showHelp testen: Job für Integrationstest, wenn App den Parser treibt?
}

TEST_F(OptionParserFixture, TestHelpViewer) {
    op->showHelp(cerr); // TODO: umleiten in Stringstream, dann abfischen
}


