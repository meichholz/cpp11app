#include "everything.h"
#include "showcase.h"

using namespace std;

// see: http://www.cplusplus.com/reference/
// see: file:///usr/share/doc/c++-annotations/html/index.html

int
main (int argc, char **argv) {
    int exit_code = 0;
    try {
        ::theApp = new App(argc, argv);
        exit_code = ::theApp->run(); // standard procedure: fire and forget
    }
    catch (exception& e) {
        cout << "standard exception caught: " << e.what() << ". dying :-(" << endl;
        exit_code = 2;
    }
    catch (...) {
        cerr << "generic exception caught. dying :-(" << endl;
        exit_code = 2;
    }
    // clean up, especially for valgrind
    if (::theApp) delete ::theApp;
    ::theApp = nullptr;
    return exit_code;
}

