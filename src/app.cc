#include "everything.h"

const char * const App::getGreeter() { return "Hello, World!"; }
App::App(int argc, char **argv) {
    commandline_arguments = new vector<string>;
    for (int i=0; i<argc; i++) {
        commandline_arguments->push_back(string(argv[i]));
    }
}

App::App(initializer_list<char*> argv)
{
    commandline_arguments = new vector<string>(argv.begin(), argv.end());
}

void App::showArguments() { for (auto arg : *commandline_arguments) { cout << "arg: " << arg << endl; } }

App::~App() {
    delete commandline_arguments; // THINK: does this free the vector content?
}

