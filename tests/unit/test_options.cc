#include "testhelper.h"

//  http://code.google.com/p/googletest/wiki/Primer#String_Comparison
//  http://code.google.com/p/googletest/wiki/AdvancedGuide#Explicit_Success_and_Failure

extern "C" {
#include <unistd.h>
#include <getopt.h>
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
                virtual void setValue(string arg, Flags flags) { cerr << "cannot set " << arg << endl; }
        };
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
            ~Record() { delete value_p_; }
            // accessors
            char getShortName() { return shortflag_; }
            const string & getLongName() { return longflag_; }
            const string & getDescription() { return description_; }
            Flags getFlags() { return flags_; }
            ValueBase & getValue() { return *value_p_; }
    };


    // }}}
    class Parser { // {{{
        public:
            Parser() {};
            virtual ~Parser() {};
            virtual void on (char o_short, char const *o_long,
                    bool * value, bool def_value=true,
                    char const *description=nullptr);
            virtual void on (char o_short, char const *o_long,
                    ValueBase *value_p,
                    char const *description=nullptr, Flags of=Flags::DEFAULT);
            virtual void parse(vector<string> args);
        private:
            vector<unique_ptr<Record>> opts_; // will safely destroy record objects
    };

    // }}}
} // namespace

void Option::Parser::parse(vector<string> args) //{{{
{
    // http://www.cplusplus.com/reference/string/string/
    string short_opts = "";
    auto c_opts=opts_.size();
    struct option *long_opts = new struct option [c_opts+1];
    // zero the last record explicitly
    long_opts[c_opts].name = nullptr;
    long_opts[c_opts].flag = nullptr;
    long_opts[c_opts].val = long_opts[c_opts].has_arg = 0;
    for (decltype(c_opts) i=0; i<c_opts; i++) // no range possibly possible
    {
        auto rec=opts_[i].get(); // get the object ptr behind
        cout << "checking: "<<rec->getDescription() << endl;
        long_opts[i].name = rec->getLongName().c_str();
        long_opts[i].val = rec->getShortName();
        long_opts[i].flag = NULL; // use value and compatibility layer
        // set short flag
        short_opts.push_back(rec->getShortName());
        if (! ((int)(rec->getFlags()) & (int)(Option::Flags::BOOLEAN) ))
        {
            // argument needed
            short_opts.push_back(':');
            long_opts[i].has_arg = required_argument;
        }
        else
        {
            long_opts[i].has_arg = no_argument;
        }
        // set default value
        rec->getValue().setDefault();
    }
    cout << "short options: " << short_opts << endl;
    // TODO 3: transform args to argc/argv
    // int argc = args.size();
    // char **argv = nullptr;
    // TODO 4: iterate through getopt_long()
    // TODO: delegate to submethod
    // cleanup
    delete [] long_opts;
}
//}}}
void Option::Parser::on // {{{
    (char o_short, char const *o_long,
     ValueBase *value_p,
     char const *description, Flags of)
{
    opts_.push_back(unique_ptr<Record>(new Record(o_short, o_long, description, of, value_p)));
}
void Option::Parser::on // {{{
    (char o_short, char const *o_long, bool *value, bool def_value,
     char const *description)
{
    auto oval = new Option::Value<bool>(value, def_value);
    auto of = Option::Flags::BOOLEAN;
    opts_.push_back(unique_ptr<Record>(new Record(o_short, o_long, description, of, oval)));
}
//}}}
TEST_F(AppFixtureBase, options) {
    Option::Parser op;
    unsigned int dmask(56u);
    bool verbose(true);
    string config_file_name("wurscht");
    op.on ('v', "verbose", &verbose, false, "verbose output");
    op.on ('c', "config", new Option::Value<string>(&config_file_name, "./x.ini"), "configfile name");
    op.on ('d', "debug", new Option::Value<unsigned int>(&dmask, 2204u), "debugging mask");

    op.parse(vector<string>({CS("egal"), CS("-v"), CS("-d"), CS("23") }));

    EXPECT_EQ(verbose, false);
    EXPECT_EQ(dmask, 2204u);
    EXPECT_STREQ(config_file_name.c_str(), "./x.ini");
}


