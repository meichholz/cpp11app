#ifndef APP_H
#define APP_H

class App {
  public:
  App(int argc, char **argv);
  ~App();
  const char * const getGreeter();
};

extern App *theApp;

#endif
