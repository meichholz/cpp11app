/*
 * =====================================================================================
 *
 *       Filename:  option.cc
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

#include "everything.h"

// TODO: callback functions

// specialized Value functions {{{
// http://www.cprogramming.com/tutorial/template_specialization.html
namespace Option { // must take place within namespace
    template<>
        void Value<string>::set(string arg, Flags flags) {
            *(d_val.first) = arg;
        }
}
// }}}
// {{{ Parser ctor stuff
Option::Parser::Parser(string appname, string argspec) :
    d_longopts(nullptr),
    d_shortopts(""),
    d_appname(appname),
    d_argspec(argspec)
{
}

Option::Parser::~Parser()
{
    cleanupGnuInterface();
}
// }}}
void
Option::Parser::setupGnuInterface()
//{{{
{
    auto c_opts=d_opts.size();
    d_longopts = new struct option [c_opts+1];
    d_shortopts = "";
    // zero the last record explicitly
    d_longopts[c_opts].name = nullptr;
    d_longopts[c_opts].flag = nullptr;
    d_longopts[c_opts].val = d_longopts[c_opts].has_arg = 0;
    for (decltype(c_opts) i=0; i<c_opts; i++) // no range possibly possible
    {
        auto rec=d_opts[i].get(); // get the object ptr behind
        d_longopts[i].name = rec->longName().c_str();
        d_longopts[i].val = rec->shortName();
        d_longopts[i].flag = NULL; // use value and compatibility layer
        d_shortopts.push_back(rec->shortName());
        if (rec->hasArgument())
        {
            d_shortopts.push_back(':');
            d_longopts[i].has_arg = required_argument;
        }
        else
        {
            d_longopts[i].has_arg = no_argument;
        }
        rec->value().useDefault();
    }
    // cout << "short options: " << shortopts_ << endl;
}
// }}}
void
Option::Parser::cleanupGnuInterface()
// {{{
{
    delete [] d_longopts;
    d_longopts = nullptr;
}
//}}}
void
Option::Parser::showHelp(std::ostream &os)
// {{{
{
    os << "usage: " << d_appname << " {OPTION} " << d_argspec << endl << endl;
    os << "OPTION may be:" << endl;
    auto c_opts = d_opts.size();
    for (decltype(c_opts) i=0; i<c_opts; i++) // no range possibly possible
    {
        auto rec=d_opts[i].get(); // get the object ptr behind
        os << "\t-" << rec->shortName() << "/--"  << rec->longName()
            << "\t: " << rec->description();
        if (rec->hasArgument()) {
          os << " [" << rec->value().defaultValue() << "]";
        }
        os << endl;
    }
}
// }}}
const Option::Record *
Option::Parser::findRecordByFlag(char oflag)
// {{{
{
    auto c_opts = d_opts.size();
    for (decltype(c_opts) i=0; i<c_opts; i++) // no range possibly possible
    {
        auto rec=d_opts[i].get(); // get the object ptr behind
        if (rec->shortName() == oflag)
            return rec;
    }
    return nullptr;
}
// }}}
/*! Run specified argument string through GNU getopt_long.
 * \param args the complete commandline (INCLUDING program name at ARGV[0])
 * \return OK or FAIL
 * */
int
Option::Parser::parse(vector<string> args)
//{{{
{
    setupGnuInterface();
    int argc = args.size();
    char *argv[argc];
    for (int i = 0; i < argc; i++) { // transscribe args to argv
        argv[i] = const_cast<char*>(args.at(i).c_str());
    }
    int index;
    auto is_parsing = true;
    optind = 1; // hard reset getopt library
    while (is_parsing) {
        int ch_num = getopt_long(argc, argv, d_shortopts.c_str(), d_longopts, &index);
        switch (ch_num) {
            case -1:
                is_parsing = false;
                break;
            case 0:
                throw std::runtime_error("Option::Parser: autoflag not allowed");
                break;
            case '?':
                cout << "unknown command line option" << endl;
                cout << "try: " << d_appname << "  --help" << endl;
                cleanupGnuInterface();
                return FAIL;
            default:
                char ch(ch_num);
                const Record * rec_p = findRecordByFlag(ch);
                if (!rec_p) throw std::runtime_error("Option::Parser: flag not found");
                const string value(optarg ? optarg : "1"); // use "1" as default value with flags
                rec_p->value().set(value, rec_p->flags());
        }
    }
    // cleanup
    cleanupGnuInterface();
    return OK;
}
//}}}
void
Option::Parser::on
//{{{ generic: ValueBase
    (
    char o_short, char const *o_long,
    ValueBase *value_p,
    char const *description, Flags of )
{
    d_opts.push_back(unique_ptr<Record>(new Record(o_short, o_long, description, of, value_p)));
}
//}}}
void
Option::Parser::on
//{{{ bool
    (
    char o_short, char const *o_long,
    bool &value, 
    char const *description )
{
    auto oval = new Option::Value<bool>(&value, false);
    auto of = Option::Flags::BOOLEAN;
    d_opts.push_back(unique_ptr<Record>(new Record(o_short, o_long, description, of, oval)));
}
//}}}
void
Option::Parser::on
//{{{ string with cs default
    (
     char o_short, char const *o_long,
     string &value,
     const char *def_value,
     char const *description,
     Option::Flags of )
{
    auto oval = new Option::Value<string>(&value, def_value);
    d_opts.push_back(unique_ptr<Record>(new Record(o_short, o_long, description, of, oval)));
}
//}}}

