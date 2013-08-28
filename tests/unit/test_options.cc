#include "testhelper.h"

//  http://code.google.com/p/googletest/wiki/Primer#String_Comparison
//  http://code.google.com/p/googletest/wiki/AdvancedGuide#Explicit_Success_and_Failure
// http://www.cplusplus.com/reference/string/string/

extern "C" {
#include <unistd.h>
#include <getopt.h>
}

typedef enum { OK, FAIL } t_rc;

// TODO: Help...

// TODO: callback functions

namespace std {
    string to_string(string &s) { return s; }
}

namespace Option {
    enum class Flags { // {{{
        DECIMAL = 1,
        HEXADECIMAL = 2,
        BINARY = 4,
        FLOAT = 8,
        OPTIONAL = 16,
        BOOLEAN = 32,
        DEFAULT = DECIMAL,
    };
    // }}}
    class ValueBase { // {{{
        public:
            virtual ~ValueBase() {}
            virtual ValueBase *clone() = 0;
            virtual void setDefault() = 0;
            virtual const string getDefault() = 0;
            virtual void setValue(string arg, Flags flags) = 0;
    };

    template <typename T>
        class Value : public ValueBase {
            private:
                std::pair<T*, T> val_; // varptr, defval
            public:
                // constructors
                Value(T *var_p, T def_value) { val_ = std::make_pair(var_p, def_value); }
                Value(Value<T> *o) { val_ = o->val_; }
                virtual ~Value() { }
                virtual Value<T> *clone() { return new Value<T>(this); }
                virtual void setDefault() { *(val_.first) = val_.second; }
                virtual const string getDefault() { return std::to_string(val_.second); }
                virtual void setValue(string arg, Flags flags);
        };

template <typename T>
void Value<T>::setValue(string arg, Flags flags)
{
    std::istringstream argstream(arg);
    T result;
    std::ios_base& (*conversion)(std::ios_base&);
    conversion = std::dec; // TODO: find conversion based on flags
    bool failed = (argstream >> conversion >> result).fail();
    if (failed) throw std::runtime_error(string("Option::Parser: cannot convert ")+arg);
    *(val_.first) = result;
}

    // }}}
    class Record { // {{{
        private:
            ValueBase *value_p_;
            Flags flags_;
            char shortflag_;
            string longflag_;
            string  description_;
        public:
            // constructors
            Record(char shortflag, string longflag, string description, Flags flags, ValueBase *value_p) : 
                value_p_(value_p),
                flags_(flags),
                shortflag_(shortflag),
                longflag_(longflag),
                description_(description)
                {}
            virtual ~Record() { delete value_p_; }
            // accessors
            char getShortName() const { return shortflag_; }
            const string & getLongName() const { return longflag_; }
            virtual const string & getDescription() const { return description_; }
            virtual Flags getFlags() const { return flags_; }
            ValueBase & getValue() const { return *value_p_; }
            // utility
            virtual bool hasArgument() const {
               return !((int)(flags_) & (int)(Option::Flags::BOOLEAN) );
            }
    };


    // }}}
    class Parser { // {{{
        public:
            Parser(string appname="program", string argspec="");
            virtual ~Parser();
            virtual void on (char o_short, char const *o_long,
                    bool * value, 
                    char const *description="");
            virtual void on (char o_short, char const *o_long,
                    ValueBase *value_p,
                    char const *description="", Flags of=Flags::DEFAULT);
            virtual int parse(vector<string> args);
            virtual void showHelp(std::ostream &os = cout);
        private:
            vector<unique_ptr<Record>> opts_; // will safely destroy record objects
            struct option *longopts_; // man 3 getopt_long
            string shortopts_;
            string appname_;
            string argspec_;
            void setupGnuInterface();
            void cleanupGnuInterface();
            const Record *findRecordByFlag(char oflag);
    };
    // }}}
} // namespace

// {{{ ctor stuff
Option::Parser::Parser(string appname, string argspec) :
    longopts_(nullptr),
    shortopts_(""),
    appname_(appname),
    argspec_(argspec)
{ }

Option::Parser::~Parser() {
    cleanupGnuInterface();
}
// }}}
void Option::Parser::setupGnuInterface() //{{{
{
    auto c_opts=opts_.size();
    longopts_ = new struct option [c_opts+1];
    shortopts_ = "";
    // zero the last record explicitly
    longopts_[c_opts].name = nullptr;
    longopts_[c_opts].flag = nullptr;
    longopts_[c_opts].val = longopts_[c_opts].has_arg = 0;
    for (decltype(c_opts) i=0; i<c_opts; i++) // no range possibly possible
    {
        auto rec=opts_[i].get(); // get the object ptr behind
        longopts_[i].name = rec->getLongName().c_str();
        longopts_[i].val = rec->getShortName();
        longopts_[i].flag = NULL; // use value and compatibility layer
        shortopts_.push_back(rec->getShortName());
        if (rec->hasArgument())
        {
            shortopts_.push_back(':');
            longopts_[i].has_arg = required_argument;
        }
        else
        {
            longopts_[i].has_arg = no_argument;
        }
        rec->getValue().setDefault();
    }
    // cout << "short options: " << shortopts_ << endl;
}
// }}}
void Option::Parser::cleanupGnuInterface() // {{{
{
    delete [] longopts_;
    longopts_ = nullptr;
}

//}}}
void Option::Parser::showHelp(std::ostream &os) // {{{
{
    os << "usage: " << appname_ << " {OPTION} " << argspec_ << endl << endl;
    os << "OPTION may be:" << endl;
    auto c_opts = opts_.size();
    for (decltype(c_opts) i=0; i<c_opts; i++) // no range possibly possible
    {
        auto rec=opts_[i].get(); // get the object ptr behind
        os << "\t-" << rec->getShortName() << "/--"  << rec->getLongName()
            << "\t: " << rec->getDescription();
        if (rec->hasArgument()) {
          os << " [" << rec->getValue().getDefault() << "]";
        }
        os << endl;
    }
}
// }}}
const Option::Record *Option::Parser::findRecordByFlag(char oflag) // {{{
{
    auto c_opts = opts_.size();
    for (decltype(c_opts) i=0; i<c_opts; i++) // no range possibly possible
    {
        auto rec=opts_[i].get(); // get the object ptr behind
        if (rec->getShortName() == oflag)
            return rec;
    }
    return nullptr;
}

int Option::Parser::parse(vector<string> args) //{{{
{
    setupGnuInterface();
    int argc = args.size();
    char *argv[argc];
    for (int i=0; i<argc; i++) { // transscribe args to argv
        argv[i]=CS(args.at(i).c_str());
    }
    int index;
    auto is_parsing = true;
    optind = 0; // hard reset getopt library
    while (is_parsing) {
        int ch_num = getopt_long(argc, argv, shortopts_.c_str(), longopts_, &index);
        switch (ch_num) {
            case -1:
                is_parsing = false;
                break;
            case 0:
                throw std::runtime_error("Option::Parser: autoflag not allowed");
                break;
            case '?':
                cout << "unknown command line option" << endl;
                cout << "try: " << appname_ << "  --help" << endl;
                cleanupGnuInterface();
                return FAIL;
            default:
                char ch(ch_num);
                const Record * rec_p = findRecordByFlag(ch);
                if (!rec_p) throw std::runtime_error("Option::Parser: flag not found");
                const string value(optarg ? optarg : "1");
                rec_p->getValue().setValue(value, rec_p->getFlags());
        }
    }
    // cleanup
    cleanupGnuInterface();
    return OK;
}
//}}}
void Option::Parser::on // {{{
    (char o_short, char const *o_long,
     ValueBase *value_p,
     char const *description, Flags of)
{
    opts_.push_back(unique_ptr<Record>(new Record(o_short, o_long, description, of, value_p)));
}

void Option::Parser::on
    (char o_short, char const *o_long, bool *value, 
     char const *description)
{
    auto oval = new Option::Value<bool>(value, false);
    auto of = Option::Flags::BOOLEAN;
    opts_.push_back(unique_ptr<Record>(new Record(o_short, o_long, description, of, oval)));
}
//}}}

class OptionParserFixture : public AppFixtureBase {
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

TEST_F(OptionParserFixture, TestOptionSetter) {
    op->parse(vector<string>({ CS("egal") }));
    EXPECT_EQ(verbose, false);
    EXPECT_EQ(dmask, 7u);
    EXPECT_STREQ(config_file_name.c_str(), "./x.ini");
    op->parse(vector<string>({CS("egal"), CS("-v"), CS("-d"), CS("23") }));
    EXPECT_EQ(verbose, true);
    EXPECT_EQ(dmask, 23u);
    EXPECT_STREQ(config_file_name.c_str(), "./x.ini");
    // TODO: string set implementieren
    // TODO: showHelp testen: Job für Integrationstest, wenn App den Parser treibt?
}

TEST_F(OptionParserFixture, TestHelpViewer) {
    op->showHelp(cerr); // TODO: umleiten in Stringstream, dann abfischen
}


