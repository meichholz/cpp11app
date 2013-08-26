#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "everything.h"

//  http://code.google.com/p/googletest/wiki/Primer#String_Comparison
//  http://code.google.com/p/googletest/wiki/AdvancedGuide#Explicit_Success_and_Failure

// OK, we *REALLY* have to study move semantics, because too many anonymous
// constructors.

#define CS(s) const_cast<char*>(s)

class App_Fixture : public ::testing::Test {
    virtual void SetUp() {
        ::theApp = new App({ CS("egal"), CS("") } );
    }
    virtual void TearDown() { delete ::theApp; ::theApp = nullptr; }
};

enum class OptionFlags {
    OF_DECIMAL = 1,
    OF_HEXADECIMAL = 2,
    OF_BINARY = 4,
    OF_FLOAT = 8,
    OF_OPTIONAL = 16,
    OF_DEFAULT = OF_DECIMAL,
};

class OptionValueBase {
    public:
        virtual string name() = 0;
        virtual ~OptionValueBase() {}
};

template <typename T>
class OptionValue : public OptionValueBase {
    private:
        std::pair<T, T&> *val_;
    public:
        virtual string name () { return "unknown"; }
        OptionValue(T& ref, T def_value) { val_ = nullptr; }
        virtual ~OptionValue() { if (val_) delete val_ ; }
};

class OptionRecord {
    public:
        OptionRecord(char shortflag, string longflags, string description, OptionFlags flags, OptionValueBase *value_p);
        ~OptionRecord();
    private:
        char shortflag_;
        string longflag_;
        string  description_;
        OptionFlags flags_;
        OptionValueBase *value_p_;
   friend class OptionParser;
};

OptionRecord::OptionRecord(char shortflag, string longflags, string description, OptionFlags flags, OptionValueBase *value_p)
{
            shortflag_= shortflag;
            flags_ = flags;
            value_p_ = value_p_; // move semantics?
            description_ = description;
}

OptionRecord::~OptionRecord()
{
    // erm...
}

class OptionParser {
    public:
        OptionParser() {};
        virtual ~OptionParser() {};
        virtual void on (char o_short, char const *o_long,
                OptionValueBase && value,
                char const *description=nullptr, OptionFlags of=OptionFlags::OF_DEFAULT);
        virtual void parse(vector<string> args);
    private:
        vector<OptionRecord> opts_;
};

void OptionParser::parse(vector<string> args)
{
    // string short_opts = "";
    // vector<const struct option> long_opts;
    for (auto o_rec : opts_) {
        // TODO 1: construct short flag string
        // TODO 2: construct long options array
        // TODO 3: transform args to argc/argv
        // TODO 4: iterate through getopt_long()
        cout << "checking flag "<<o_rec.shortflag_ << endl;
    }

}

void OptionParser::on (char o_short, char const *o_long,
       OptionValueBase && value,
       char const *description, OptionFlags of)
{
    cout << "registering a generic tuple '"<< value.name()<<"'" << endl;
    opts_.push_back(OptionRecord(o_short, o_long, description, of, &value));
}

TEST_F(App_Fixture, options) {
    OptionParser op;
    unsigned int dmask;
    bool verbose;
    string config_file_name;
    op.on ('v', "verbose", OptionValue<bool>(verbose, false), "verbose output");
    op.on ('c', "config", OptionValue<string>(config_file_name, "./x.ini"), "configfile name");
    op.on ('d', "debug", OptionValue<unsigned int>(dmask, 0u), "debugging mask");

    op.parse(vector<string>({CS("egal"), CS("-v"), CS("-d"), CS("23") }));
    
    FAIL();
}


