#include "everything.h"
#include "showcase.h"

using namespace std;

App *theApp=(App*)0;

// see: http://www.cplusplus.com/reference/
// see: file:///usr/share/doc/c++-annotations/html/index.html

int
main (int argc, char *argv[])
{
    try {
        ::theApp = new App(argc, argv);
        if (argc==2 && string("--testrun=showcase")==argv[1]) {
            ::theApp->showArguments();
            /* provisorial shortcut to showcase */
            auto showcase = new Showcase();
            showcase->run();
            showcase->throw_simple();
        }
    }
  catch (exception& e)
  {
    cout << "standard exception caught: " << e.what() << ". dying :-(" << endl;
  }
  catch (...) {
      cerr << "generic exception caught. dying :-(" << endl;
  }
  return 0;
}

