/*
 * =====================================================================================
 *
 *       Filename:  everything.h
 *
 *    Description:  Include JUST THIS file for the complete environment
 *
 *        Version:  1.0
 *        Created:  08/28/13 16:15:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Marian Eichholz (me), marian@bugslayer.de
 *   Organization:  freenet.de GmbH
 *
 * =====================================================================================
 */
#ifndef EVERYTHING_H
#define EVERYTHING_H

// system header
#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <initializer_list>
#include <utility>
#include <boost/regex.hpp>

extern "C" {
#include <unistd.h>
#include <getopt.h>
} // for option.h

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::initializer_list;
using std::unique_ptr;
using std::shared_ptr;

typedef enum { OK, FAIL } t_rc;

// goodies in globals.cc
namespace std {
    string to_string(string &s);
}

char * operator "" _cs(const char *cstr, size_t len);

// application own header
#include "app.h"
#include "option.h"

// #pragma hdrstop

#endif
