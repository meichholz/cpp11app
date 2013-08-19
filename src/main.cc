#include "app.h"
#include "showcase.h"

#include <iostream>

App *theApp=(App*)0;

int
main (int argc, char **argv)
{
  ::theApp = new App(argc, argv);
  /* shortcut to showcase */
  auto showcase = new Showcase();
  showcase->run();
  return 0;
}

