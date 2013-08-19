#include <iostream>
#include <cmath>
#include <vector>

#include "showcase.h"

static void showSimpleIteration()
{
    int myarray[5]={ 1,2,3,4,5};
    for (auto &x : myarray) x*=2;
    for (auto x: myarray) std::cout << x << " ";
    std::cout << std::endl;
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
        std::cout << "hour=" << hour << std::endl;
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
            std::cout << "IcBase::IcBase("<< i << std::endl;
        }
        IcBase(std::string s) {
            std::cout << "IcBase::IcBase("<< s << std::endl;
        }
};

class IcDerived: public IcBase {
    public:
        using IcBase::IcBase;
        IcDerived(double d) {
            std::cout << "IcDerived::IcDerived(" << d << std::endl;
        }
};

void showInheritedConstructor() {
    IcDerived(2011);
    IcDerived("C++11");
    IcDerived(0.33);
}
#else
void showInheritedConstructor() { std::cout << "inherited constructor not available" << std::endl; }
#endif

void Showcase::run()
{
    showInheritedConstructor();
    showSimpleIteration();
    showValidatorDelegation();
}


