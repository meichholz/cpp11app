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
        App();
        App(int argc, char **argv);
        App(initializer_list<char*> argv);
        ~App();
        int run();
        // debugging
        const char * const getGreeter();
    protected:
        AppMode setup(Option::Parser& parser);
    private:
        vector<string> *d_commandline_arguments;
        string          d_config_filename;
        unsigned int    d_dmask;
        bool            d_verbose;

        void            showArguments();
};

extern App *theApp;

#endif
