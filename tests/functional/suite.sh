#!/bin/bash

THIS="$0"

. `dirname "$THIS"`/boilerplate.sh

# of course we could pre-test sanity conditions to not-call the
# test framework anyway. But that seems not too integrated with the
# framework idea at all.

SUT_OK=yes

test_aaa_sanity()
{
	if [ ! -x "$SUT" ]; then
		fail "SUT not available, aborting SUITE"
		# startSkipping
		SUT_OK=no
	fi
}

setUp()
{
	# if SUT is not OK, then we skip everything.
	# should be tested early, therefore
	if [ "$SUT_OK" != yes ]; then
		startSkipping
	fi
}

rem_test_usage()
{
	assertMatches "does not greet" "$SUT -h" 'cppeleven'
	assertMatches "shows no version" "$SUT -V" 'version [0-9]'
	assertFalse "accepts wrong option" "$SUT --frobnifyworld"
}

test_inifile()
{
	assertTrue "cannot set config file name" "$SUT -c $BASEDIR/test.ini"
}

runTests $THIS "$LINENO"



