#include "app.h"
#include "showcase.h"

#include <iostream>

using namespace std;

App *theApp=(App*)0;

// see: http://www.cplusplus.com/reference/

int
main (int argc, char **argv)
{
  try {
      ::theApp = new App(argc, argv);
      /* shortcut to showcase */
      auto showcase = new Showcase();
      showcase->run();
      showcase->throw_simple();
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

