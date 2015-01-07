/*
 * =====================================================================================
 *
 *       Filename:  option.h
 *
 *    Description:  Generic Option Parser
 *
 *        Version:  1.0
 *        Created:  08/28/13 15:33:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Marian Eichholz (me), marian@bugslayer.de
 *   Organization:  freenet.de GmbH
 *
 * =====================================================================================
 */

#ifndef OPTION_H
#define OPTION_H

namespace Option {
    enum class Flags { // {{{
        DECIMAL = 1,
        HEXADECIMAL = 2,
        BINARY = 4,
        FLOAT = 8,
        OPTIONAL = 16,
        BOOLEAN = 32,
        STRING  = 64,
        FILENAME = 128,
        DEFAULT = DECIMAL,
    };
    // }}}
    class ValueBase { // {{{
        public:
            virtual ~ValueBase() {}
            virtual ValueBase *clone() = 0;
            virtual void useDefault() = 0;
            virtual const string defaultValue() const = 0;
            virtual void set(string arg, Flags flags) = 0;
    };

    template <class T>
        class Value : public ValueBase {
            private:
                std::pair<T*, T> d_val; // varptr, defval
            public:
                // constructors
                Value(T *var_p, T & def_value) {  d_val = std::make_pair(var_p, def_value); }
                Value(T *var_p, T && def_value) {  d_val = std::make_pair(var_p, std::move(def_value)); }
                Value(Value<T> *o) { d_val = o->d_val; }
                virtual ~Value() { }
                virtual Value<T> *clone() { return new Value<T>(this); }
                virtual void useDefault() { *(d_val.first) = d_val.second; }
                virtual const string defaultValue() const { return std::to_string(d_val.second); }
                virtual void set(string arg, Flags flags);
        };

template <class T>
void Value<T>::set(string arg, Flags flags)
{
    std::istringstream argstream(arg);
    T result;
    std::ios_base& (*conversion)(std::ios_base&);
    conversion = std::dec; // TODO: find conversion based on flags
    bool failed = (argstream >> conversion >> result).fail();
    if (failed) throw std::runtime_error(string("Option::Parser: cannot convert ")+arg);
    *(d_val.first) = result;
}
// }}}
    class Record { // {{{
        private:
            ValueBase *d_pvalue;
            Flags d_flags;
            char d_shortflag;
            string d_longflag;
            string  d_description;
        public:
            // constructors
            Record(char shortflag, string longflag, string description, Flags flags, ValueBase *pvalue) : 
                d_pvalue(pvalue),
                d_flags(flags),
                d_shortflag(shortflag),
                d_longflag(longflag),
                d_description(description)
                {}
            virtual ~Record() { delete d_pvalue; }
            // accessors
            char shortName() const { return d_shortflag; }
            const string & longName() const { return d_longflag; }
            virtual const string & description() const { return d_description; }
            virtual Flags flags() const { return d_flags; }
            ValueBase & value() const { return *d_pvalue; }
            // utility
            virtual bool hasArgument() const {
               return !((int)(d_flags) & (int)(Option::Flags::BOOLEAN) );
            }
    };


    // }}}
    class Parser { // {{{
        private:
            vector<unique_ptr<Record>> d_opts; // will safely destroy record objects
            struct option *d_longopts; // man 3 getopt_long
            string d_shortopts;
            string d_appname;
            string d_argspec;
        public:
            Parser(string appname="program", string argspec="");
            virtual ~Parser();
            // for booleans
            virtual void on (char o_short, char const *o_long,
                    bool & value, 
                    char const *description="");
            // for strings
            virtual void on (char o_short, char const *o_long,
                    string &value, char const *def_value,
                    char const *description="",
                    Flags of=Flags::STRING);
            // for ValueBase objects
            virtual void on (char o_short, char const *o_long,
                    ValueBase *value_p,
                    char const *description="",
                    Flags of=Flags::DEFAULT);
            // generic on demand version for templatable Values
            template<typename T>
            void on (char o_short, char const *o_long,
                    T & value, T && def_value,
                    char const *description="",
                    Flags of=Flags::DEFAULT);
            virtual int parse(vector<string> args);
            virtual void showHelp(std::ostream &os = cout);
        private:
            void setupGnuInterface();
            void cleanupGnuInterface();
            const Record *findRecordByFlag(char oflag);
    };
    // }}}

template <typename T>
void Parser::on (char o_short, char const *o_long,
        T & value, T && def_value,
        char const *description, Flags of)
{
    auto oval = new Value<T>(&value, def_value);
    d_opts.push_back(unique_ptr<Record>(new Record(o_short, o_long, description, of, oval)));
}

} // namespace

#endif

