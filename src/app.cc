#include "everything.h"

const char * const App::getGreeter() { return "Hello, World!"; }
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

void App::run()
{
    string programname = "cppeleven";
    string version = "0.0.1";
    string trailer = "MODES:\n\tshowcase : run the showcase suite.\n\tconfig : show configuration.\n"; // TODO use some command/mode abstraction
    Option::Parser parser(programname, "<mode>");
    AppMode mode = setup(parser);
    switch (mode) {
        case AppMode::usage:
            cout << programname << " version " << version << endl << endl;
            parser.showHelp(cout); // probably comment on modes
            cout << trailer << endl;
            break;
        case AppMode::version:
            cout << programname << " version " << version << endl;
            break;
        case AppMode::showcase:
            cerr << "showcase not integrated yet" << endl;
            break;
        default:
            cout << "doing nothing for now" << endl;
    }
}

AppMode App::setup(Option::Parser &parser)
{
    unsigned int dmask = 0x0000u;
    bool verbose = false;
    // TODO need real callback blocks
    bool want_help = false;
    bool want_version = false;
    // --- set up parser
    parser.on ('v', "verbose", verbose, "verbose output");
    parser.on ('V', "version", want_version, "show version number");
    parser.on<unsigned int>('d', "debug", dmask, 0x0000u, "debugging mask");
    parser.on ('h', "help", want_help, "show options and modes");
    // --- run the command line and handle error
    if (OK != parser.parse(*d_commandline_arguments)) {
        cerr << "option parser failed." << endl;
        parser.showHelp(cerr); // probably comment on modes
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

