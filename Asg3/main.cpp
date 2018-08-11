// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $
// Connor Thurmond (cthurmon) Rami Alrwais (raalrwai)

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unistd.h>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
    opterr = 0;
    for (;;) {
        int option = getopt (argc, argv, "@:");
        if (option == EOF) break;
        switch (option) {
            case '@':
                debugflags::setflags (optarg);
                break;
            default:
                complain() << "-" << char (optopt) << ": invalid option"
                              << endl;
                break;
        }
    }
}

void process_file (istream& infile, const string& filename, 
    listmap<string, string>& test) {
        regex comment_regex {R"(^\s*(#.*)?$)"};
        regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
        regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
        int line_counter = 1;
        for (;;) {
            string line;
            getline (infile, line);
            if (infile.eof()) break;
            cout << filename << ": " << line_counter << ": " << line
                << endl;
            smatch result;
            if (regex_search (line, result, comment_regex)) {
                ++line_counter;
                continue;
            }
            if (regex_search (line, result, key_value_regex)) {
                if (result[1] == "" && result[2] == "") {
                    for (auto i = test.begin(); i != test.end(); ++i) {
                        cout << i->first << " = " << i->second << endl;
                    }
                } else if (result[2] == "") {
                    str_str_map::iterator del_pos = 
                        test.find(result[1]);
                    test.erase(del_pos);
                } else if (result[1] == "") {
                    for (auto i = test.begin(); i != test.end(); ++i) {
                        if (i->second == result[2]) {
                            cout << i->first << " = " << i->second 
                                << endl;
                        }
                    }
                } else {
                    str_str_pair insert_pair (result[1], result[2]);
                    test.insert (insert_pair);
                }
            } else if (regex_search (line, result, trimmed_regex)) {
                str_str_map::iterator find_pos = test.find(result[1]);
                if (find_pos == test.end()) {
                    cout << result[1] << ": key not found" << endl;
                } else {
                    cout << find_pos->first << " = " << 
                        find_pos->second << endl;
                }
            } else {
                cout << "This cannot happen." << endl;
            }
            ++line_counter;
        }
}

int main (int argc, char** argv) {
    sys_info::execname (argv[0]);
    scan_options (argc, argv);
    int exit_status = EXIT_SUCCESS;
    str_str_map test;
    const string cin_name = "-";

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (argv[i] == cin_name) {
                process_file(cin, cin_name, test);
            } else {
                ifstream infile (argv[i]);
                if (infile.fail()) {
                    exit_status = 1;
                    cerr << argv[0] << ": " << argv[i] << 
                        ": No such file or directory" << endl;
                    exit_status = 1;  
                } else {
                    process_file(infile, argv[i], test);
                    infile.close();
                }
            }
        }
    } else {
        process_file(cin, cin_name, test);
    }
    return exit_status;
}

