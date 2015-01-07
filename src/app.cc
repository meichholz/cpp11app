#include "everything.h"

const char * const App::getGreeter() { return "Hello, World!"; }

App::App()  : config_filename("/etc/cppeleven.conf")
            , dmask(0x0000u)
            , verbose(false)
{
}

App::App(int argc, char **argv) {
    d_commandline_arguments = new vector<string>;
    for (int i=0; i<argc; i++) {
        d_commandline_arguments->push_back(string(argv[i]));
    }
}

App::App(initializer_list<char*> argv)
{
    d_commandline_arguments = new vector<string>(argv.begin(), argv.end());
}

int App::run()
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
            return 1;
        case AppMode::showcase:
            cerr << "showcase not integrated yet" << endl;
            break;
        default:
            cout << "doing nothing for now" << endl;
    }
    return 0;
}

AppMode App::setup(Option::Parser &parser)
{
    // TODO need real callback blocks
    bool want_help = false;
    bool want_version = false;
    // --- set up parser
    parser.on ('v', "verbose", verbose, "verbose output");
    parser.on ('V', "version", want_version, "show version number");
    parser.on ('h', "help", want_help, "show options and modes");
    parser.on ('c', "config", config_filename, "/etc/cppeleven.conf", "use config file", Option::Flags::FILENAME);
    parser.on<unsigned int>('d', "debug", dmask, 0x0000, "debugging mask");
    // --- run the command line and handle error
    if (OK != parser.parse(*d_commandline_arguments)) {
        return AppMode::fail;
    }
    if (want_help) {
        return AppMode::usage;
    }
    if (want_version) {
        return AppMode::version;
    }
    return AppMode::none;
}

void App::showArguments() { for (auto arg : *d_commandline_arguments) { cout << "arg: " << arg << endl; } }

App::~App() {
    delete d_commandline_arguments; // THINK: does this free the vector content?
}

