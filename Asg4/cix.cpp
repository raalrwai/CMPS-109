// $Id: cix.cpp,v 1.6 2018-07-26 14:18:32-07 - - $
// Connor Thurmond (cthurmon) Rami Alrwais (raalrwai)

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream log (cout);
struct cix_exit: public exception {};

unordered_map<string,cix_command> command_map {
    {"exit", cix_command::EXIT},
    {"help", cix_command::HELP},
    {"ls"  , cix_command::LS  },
    {"put" , cix_command::PUT },
    {"get" , cix_command::GET },
    {"rm"  , cix_command::RM  },
};

static const string help = R"||(
exit         - Exit the program.  Equivalent to EOF.
get filename - Copy remote file to local host.
help         - Print help summary.
ls           - List names of files on remote server.
put filename - Copy local file to remote host.
rm filename  - Remove file from remote server.
)||";

vector<string> split (const string& line, const string& delimiters) {
    vector<string> words;
    size_t end = 0;
    for (;;) {
        size_t start = line.find_first_not_of (delimiters, end);
        if (start == string::npos) break;
        end = line.find_first_of (delimiters, start);
        words.push_back (line.substr (start, end - start));
    }
    return words;
}

void cix_help() {
    cout << help;
}

void cix_ls (client_socket& server) {
    cix_header header;
    header.command = cix_command::LS;
    log << "sending header " << header << endl;
    send_packet (server, &header, sizeof header);
    recv_packet (server, &header, sizeof header);
    log << "received header " << header << endl;
    if (header.command != cix_command::LSOUT) {
        log << "sent LS, server did not return LSOUT" << endl;
        log << "server returned " << header << endl;
    } else {
        char buffer[header.nbytes + 1];
        recv_packet (server, buffer, header.nbytes);
        log << "received " << header.nbytes << " bytes" << endl;
        buffer[header.nbytes] = '\0';
        cout << buffer;
    }
}

void cix_get (client_socket& server, const string filename) {
    cix_header header;
    header.command = cix_command::GET;
    strncpy(header.filename, filename.c_str(), sizeof(header.filename));
    send_packet (server, &header, sizeof header);
    recv_packet (server, &header, sizeof header);
    if (header.command == cix_command::NAK) {
        cout << "There was a problem copying the file over." << endl;
        cout << "Error: " << errno << endl;
        cout << "Perhaps the file doesn't exist." << endl;
    } else {
        if (header.nbytes == 0) {
            ofstream outfile;
            outfile.open(header.filename);
            outfile.close();
            cout << "File successfully copied to local directory." 
                << endl;
        } else {
            char buffer[header.nbytes];
            recv_packet (server, buffer, header.nbytes);
            buffer_to_file(buffer, filename ,header.nbytes);
            cout << "File successfully copied. to local directory."
                << endl;
        }
    }
}

void cix_put (client_socket& server, const string filename) {
    try {
        cix_header header;
        header.command = cix_command::PUT;
        strncpy(header.filename, filename.c_str(), 
            sizeof(header.filename));
        ifstream myfile(filename, ifstream::binary);
        myfile.seekg(0, myfile.end);
        int length = myfile.tellg();
        myfile.seekg(0, myfile.beg);
        char * buffer = new char [length];
        myfile.read(buffer, length);
        myfile.close();
        header.nbytes = length;
        send_packet (server, &header, sizeof header);
        send_packet (server, buffer, length);
        cix_header recv_header;
        recv_packet (server, &recv_header, sizeof recv_header);
        if (recv_header.command == cix_command::ACK) {
            cout << "File successfully copied to remote directory." 
                << endl;
        } else if (recv_header.command == cix_command::NAK) {
            cout << "There was a problem copying the file." << endl;
            cout << "Error: " << errno << endl;
        }
    } catch (...) {
        cout << "There was a problem copying the file." << endl;
        cout << "Error " << errno << endl;
        cout << "Perhaps the file doesn't exist." << endl;
    }
}

void cix_rm (client_socket& server, const string filename) {
    cix_header header;
    header.command = cix_command::RM;
    strncpy(header.filename, filename.c_str(), sizeof(header.filename));
    send_packet (server, &header, sizeof header);
    cix_header recv_header;
    recv_packet (server, &recv_header, sizeof recv_header);
    if (recv_header.command == cix_command::ACK) {
        cout << "File successfully removed from remote directory." 
            << endl;
    } else if (recv_header.command == cix_command::NAK) {
        cout << "There was a problem removing the file." << endl;
        cout << "Error: " << errno << endl;
    }
}


void usage() {
    cerr << "Usage: " << log.execname() << " [host] [port]" << endl;
    throw cix_exit();
}

int main (int argc, char** argv) {
    log.execname (basename (argv[0]));
    log << "starting" << endl;
    vector<string> args (&argv[1], &argv[argc]);
    if (args.size() > 2) usage();
    string host = get_cix_server_host (args, 0);
    in_port_t port = get_cix_server_port (args, 1);
    log << to_string (hostinfo()) << endl;
    try {
        log << "connecting to " << host << " port " << port << endl;
        client_socket server (host, port);
        log << "connected to " << to_string (server) << endl;
        for (;;) {
            string line;
            getline (cin, line);
            vector<string> command_line = split(line, " ");
            string find_command = command_line[0];
            if (cin.eof()) throw cix_exit();
            log << "command " << line << endl;
            const auto& itor = command_map.find (find_command);
            cix_command cmd = itor == command_map.end()
                        ? cix_command::ERROR : itor->second;
            switch (cmd) {
                case cix_command::EXIT:
                    if (command_line.size() != 1) {
                        cout << "exit: doesn't take arguments." << endl;
                    } else {
                        throw cix_exit();
                    }
                    break;
                case cix_command::HELP:
                    if (command_line.size() != 1) {
                        cout << "help: doesn't take arguments." << endl;
                    } else {
                        cix_help();
                    }
                    break;
                case cix_command::LS:
                    if (command_line.size() != 1) {
                        cout << "ls: doesn't take arguments." << endl;
                    } else {
                        cix_ls (server); 
                    }
                    break;
                case cix_command::PUT:
                    if (command_line.size() != 2) {
                        cout << "put: requires 1 argument." << endl;
                    } else {
                        cix_put (server, command_line[1]);
                    }
                    break;
                case cix_command::GET:
                    if (command_line.size() != 2) {
                        cout << "get: requires 1 argument." << endl;
                    } else {
                        cix_get (server, command_line[1]);
                    }
                    break;
                case cix_command::RM:
                    if (command_line.size() != 2) {
                        cout << "rm: requires 1 argument." << endl;
                    } else {
                        cix_rm(server, command_line[1]);
                    }
                    break;
                default:
                    log << line << ": invalid command" << endl;
                    break;
            }
        }
    }catch (socket_error& error) {
        log << error.what() << endl;
    }catch (cix_exit& error) {
        log << "caught cix_exit" << endl;
    }
    log << "finishing" << endl;
    return 0;
}

