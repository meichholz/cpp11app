#include "everything.h"

#include "showcase.h" // out of regular model, "ad hoccy"

const char * const App::getGreeter() { return "Hello, World!"; }

App::App()  : d_config_filename("/etc/cppeleven.conf")
            , d_dmask(0x0000u)
            , d_verbose(false)
{
}

App::App(int argc, char **argv)
{
    d_commandline_arguments = new vector<string>;
    for (int i=0; i<argc; i++) {
        d_commandline_arguments->push_back(string(argv[i]));
    }
}

App::App(initializer_list<char*> argv)
{
    d_commandline_arguments = new vector<string>(argv.begin(), argv.end());
}

//! @return proposed exit code for the application
int
App::run()
{
    string programname = "cppeleven";
    string version = "0.0.1";
    string trailer = "MODES:\n\tshowcase : run the showcase suite.\n\tconfig : show configuration.\n"; // TODO use some command/mode abstraction
    // create parser and run it
    Option::Parser parser(programname, "<mode>");
    AppMode mode = setup(parser);
    // do what the user wants
    switch (mode) {
        case AppMode::usage:
            cout << programname << " version " << version << endl << endl;
            parser.showHelp(cout); // probably comment on modes
            cout << trailer << endl;
            break;
        case AppMode::version:
            cout << programname << " version " << version << endl;
            break;
        case AppMode::fail:
            cerr << "FATAL: usage error" << endl << endl;
            parser.showHelp(cerr); // probably comment on modes
            return 3;
        case AppMode::showcase:
            {
                Showcase showcase;
                showcase.run();
                showcase.throw_simple();
            }
            return 1; // not reached
        default:
            cout << "doing nothing for now" << endl;
    }
    return 0;
}

AppMode
App::setup(Option::Parser &parser)
{
    // TODO need real callback blocks<F9>
    bool want_help = false;
    bool want_version = false;
    // --- set up parser
    parser.on ('V', "version", want_version, "show version number");
    parser.on ('h', "help", want_help, "show options and modes");
    parser.on ('v', "verbose", d_verbose, "verbose output");
    parser.on ('c', "config", d_config_filename, "/etc/cppeleven.conf", "use config file", Option::Flags::FILENAME);
    parser.on<unsigned int>('d', "debug", d_dmask, 0x0000, "debugging mask");
    // --- run the command line and handle error
    if (OK != parser.parse(*d_commandline_arguments)) return AppMode::fail;
    if (want_help) return AppMode::usage;
    if (want_version) return AppMode::version;
    // OK, we want some mapping or commandline executor class here
    if (d_commandline_arguments && !d_commandline_arguments->empty()) {
      string mode(d_commandline_arguments->back());
      if (!mode.compare("showcase"_cs)) return AppMode::showcase;
      return AppMode::fail;
    }
    return AppMode::none;
}

// for debugging only
void
App::showArguments()
{
    for (auto arg : *d_commandline_arguments) {
        cout << "arg: " << arg << endl;
    }
}

App::~App()
{
    delete d_commandline_arguments; // THINK: does this free the vector content?
}

