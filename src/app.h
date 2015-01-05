#ifndef APP_H
#define APP_H

#include "option.h"

enum AppMode {
    fail,
    none,
    usage,
    version,
    config,
    showcase,
};

class App {
    public:
        App(int argc, char **argv);
        App(initializer_list<char*> argv);
        ~App();
        const char * const getGreeter();
        // debugging
        void showArguments();
        void run();

    private:
        vector<string> *d_commandline_arguments;

    protected:
        AppMode setup(Option::Parser& parser);
};

extern App *theApp;

#endif
