#ifndef APP_H
#define APP_H

class App {
    private:
        vector<string> *d_commandline_arguments;
    public:
        App(int argc, char **argv);
        App(initializer_list<char*> argv);
        ~App();
        const char * const getGreeter();
        // debugging
        void showArguments();
};

extern App *theApp;

#endif
