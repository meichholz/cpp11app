/*
 * =====================================================================================
 *
 *       Filename:  globals.cc
 *
 *    Description:  Small helper, utilities, global data objects
 *
 *        Version:  1.0
 *        Created:  08/28/13 16:14:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Marian Eichholz (me), marian@bugslayer.de
 *   Organization:  freenet.de GmbH
 *
 * =====================================================================================
 */

#include "everything.h"

App * theApp = nullptr;

static class {
    private:
        vector<unique_ptr<string>> gs;
    public:
        char *reg(const char *cstr, size_t len) {
            auto snew = new string(cstr,len);
            gs.push_back(unique_ptr<string>(snew));
            return const_cast<char*>(snew->c_str());
        }
} keeper; // starts BEFORE everything, and ends AFTER everything, effectively freeing up all ressources.

// by a "foo"_cs literal, we get a writable copy without need to cast to
// (char*). Seems a bit overdone, yes, but the syntax is WAY clearer AND safer!
char * operator "" _cs(const char *cstr, size_t len)
{
    // cerr << "registering cs: " << cstr << endl;
    return ::keeper.reg(cstr, len);
}

namespace std {
    string to_string(string &s)
    {
        return s;
    }
}

