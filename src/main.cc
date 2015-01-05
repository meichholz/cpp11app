#include "everything.h"
#include "showcase.h"

using namespace std;

// see: http://www.cplusplus.com/reference/
// see: file:///usr/share/doc/c++-annotations/html/index.html

int
main (int argc, char **argv)
{
    try {
        ::theApp = new App(argc, argv);
        if (argc==2 && string("--testrun=showcase")==argv[1]) {
            ::theApp->showArguments();
            /* provisorial shortcut to showcase */
            Showcase showcase;
            showcase.run();
            showcase.throw_simple();
        } else {
            ::theApp->run(); // standard procedure: fire and forget
        }
    }
  catch (exception& e)
  {
    cout << "standard exception caught: " << e.what() << ". dying :-(" << endl;
  }
  catch (...) {
      cerr << "generic exception caught. dying :-(" << endl;
  }
  if (::theApp) delete ::theApp;
  ::theApp = nullptr;
  return 0;
}

