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
// }}}
int Option::Parser::parse(vector<string> args) //{{{
{
    setupGnuInterface();
    int argc = args.size();
    char *argv[argc];
    for (int i=0; i<argc; i++) { // transscribe args to argv
        argv[i]=const_cast<char*>(args.at(i).c_str());
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


