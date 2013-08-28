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

#endif

