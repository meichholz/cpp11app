#ifndef APP_H
#define APP_H

class App {
    vector<string> *commandline_arguments;
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
