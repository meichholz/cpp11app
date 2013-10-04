
# re-evaluate base directory. will be "." in CWD
BASEDIR=`dirname "$0"`

# emulate missing environment for direct invocations
# with "make check", top_builddir will be exported (or ... not)

test -z "$top_builddir" && top_builddir="$BASEDIR/../../build_dir"

# get SUT standard path
SUT="$top_builddir/src/cppeleven"

# set standard output file
OUTFILE=test.out

# ########### own macros ###########################################

assertMatches()
{
	local status
	isSkipping && return ${SHUNIT_FALSE}
	$2 >$OUTFILE
	status=$?
	assertEquals "command failed: $1" 0 $status
	assertTrue "$1: <$3> not in <`cat $OUTFILE`>" "egrep '$3' < $OUTFILE"
}


# ########### run framework #######################################
# we try to force some gcc like error report for Vim/TDD, which needs some
# wrapping to output a fake error message suitable for vim/c
# ########### run framework #######################################

# shunit2 must be sourced without argument variables: empty function
invokeShunit()
{
	# since shunit2 will exit() directly, we put it into a subshell
    ( . shunit2 )
	return $?
}

runTests()
{
	local filename lineno rc
	filename="$1"
	lineno="$2"
    
	invokeShunit
	rc="$?"

	if [ "$rc" = 0 ] ; then
		echo "OK"
	else
		test -z "$lineno" && lineno=1 # wrong lno is better than nothing
		echo "$filename:$lineno:1 error: test failed (rc=$rc, see above)"
		echo "FAILED (failures>=1)"
	fi
	exit $rc
}

