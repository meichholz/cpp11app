#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <regex>

#include "showcase.h"

using namespace std;

static void showSimpleIteration()
{
    int myarray[5]={ 1,2,3,4,5};
    for (auto &x : myarray) x*=2;
    for (auto x: myarray) cout << x << " ";
    cout << endl;
}

/* validator delegation */
class MyHour {
    int hour;
    public:
    MyHour(int hour_in) {
        if ((0<=hour_in) and (hour_in<=23))
            hour=hour_in;
        else
            hour=0;
        cout << "hour=" << hour << endl;
    }
    MyHour(): MyHour(0) {};
    MyHour(double hour_in)
        :MyHour(static_cast<int>(ceil(hour_in))) {};
};

static void showValidatorDelegation()
{
  MyHour(10);  
  MyHour(100);
  MyHour();
  MyHour(22.45);
}

#if __GNU_C__ == 4 && __GNU_C_MINOR__ >= 7
#define GCC_SUPPORTS_INHERITED_CONSTRUCTORS
#endif

#ifdef GCC_SUPPORTS_INHERITED_CONSTRUCTORS
class IcBase {
    public:
        IcBase(int i) {
            cout << "IcBase::IcBase("<< i << endl;
        }
        IcBase(string s) {
            cout << "IcBase::IcBase("<< s << endl;
        }
};

class IcDerived: public IcBase {
    public:
        using IcBase::IcBase;
        IcDerived(double d) {
            cout << "IcDerived::IcDerived(" << d << endl;
        }
};

void showInheritedConstructor() {
    IcDerived(2011);
    IcDerived("C++11");
    IcDerived(0.33);
}
#else
void showInheritedConstructor() { cout << "inherited constructor not available" << endl; }
#endif

void showRawStringLiteral()
{
    cout << R"(Eins \ zwei \n
        drei)" << endl;
    cout << R"egal(Eins "()" zwei \ drei )egal" << endl;
}

void showBasicRegex()
{
    // http://www.cplusplus.com/reference/regex/basic_regex/
    // regex are ECMA script
    // see http://solarianprogrammer.com/2011/10/12/cpp-11-regex-tutorial/
    regex rx("<marian");
    string candis[] = { "<marian@freenet.de>", "<mariaaan@freenet.de>", "<jens@freenet.de>", "marian", "maria+n" };
    // ok, regex_search() is ==def false for now (gcc 4.7.2).
    for ( auto &testme : candis ) {
        cout << "testing " << testme << ": ";
        if (regex_search(testme, rx))
            cout << "is";
        else
            cout << "not";
        cout << " matching." << endl;
    }
}

void Showcase::run()
{
    showInheritedConstructor();
    showSimpleIteration();
    showValidatorDelegation();
    showRawStringLiteral();
    showBasicRegex();
}

void Showcase::throw_simple()
{
    // http://www.cplusplus.com/reference/stdexcept/runtime_error/
    throw runtime_error("showtime in showcase");
}



