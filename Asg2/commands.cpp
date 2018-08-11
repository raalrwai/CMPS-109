// $Id: commands.cpp,v 1.17 2018-01-25 14:02:55-08 - - $
// Connor Thurmond (cthurmon) Rami Alrwais (raalrwai)

#include "commands.h"
#include "debug.h"
#include <algorithm>
#include <sstream>

command_hash cmd_hash {
    {"cat"   , fn_cat   },
    {"cd"    , fn_cd    },
    {"echo"  , fn_echo  },
    {"exit"  , fn_exit  },
    {"ls"    , fn_ls    },
    {"lsr"   , fn_lsr   },
    {"make"  , fn_make  },
    {"mkdir" , fn_mkdir },
    {"prompt", fn_prompt},
    {"pwd"   , fn_pwd   },
    {"rm"    , fn_rm    },
    {"rmr"   , fn_rmr   },
};

command_fn find_command_fn (const string& cmd) {
    // Note: value_type is pair<const key_type, mapped_type>
    // So: iterator->first is key_type (string)
    // So: iterator->second is mapped_type (command_fn)
    DEBUGF ('c', "[" << cmd << "]");
    const auto result = cmd_hash.find (cmd);
    if (result == cmd_hash.end()) {
        throw command_error (cmd + ": no such function");
    }
    return result->second;
}

command_error::command_error (const string& what):
    runtime_error (what) {
}

function_error::function_error (const string& what):
    runtime_error (what) {
}

int exit_status_message() {
    int exit_status = exit_status::get();
    cout << execname() << ": exit(" << exit_status << ")" << endl;
    return exit_status;
}

void fn_cat (inode_state& state, const wordvec& words){
    string filename;
    string new_filenmame;
    inode_ptr cwd_ptr = state.get_cwd();
    base_file_ptr cwd_contents = cwd_ptr->get_contents();
    inode_ptr file_ptr;
    wordvec data;
    base_file_ptr data_contents;
    for (size_t i = 1; i <= words.size() - 1; ++i) {
        filename = words[i];
        new_filenmame = swap_case(filename);
        file_ptr = cwd_contents->inode_from_name(new_filenmame);
        if (file_ptr) {
            data_contents = file_ptr->get_contents();
            data = data_contents->readfile();
            cout <<  word_range (data.cbegin(), data.cend()) << endl;
        } else {
            throw function_error(words[0] + ": " + words[i] +
                ": No such file or directory");
            break;
        }
    }
    DEBUGF ('c', state);
    DEBUGF ('c', words);
}

void fn_cd (inode_state& state, const wordvec& words){
    string dirname = words[1];
    string new_dirname = swap_case(dirname);
    inode_ptr curr = state.get_cwd();
    base_file_ptr content = curr->get_contents();
    inode_ptr new_cwd;
    if (words[1] == ".") {
        new_cwd = content->inode_from_name(new_dirname);
    } else if (words[1] == "..") {
        new_cwd = content->inode_from_name(new_dirname);
        state.cd_pop();
    } else {
        new_cwd = content->inode_from_name(new_dirname);
        string name = words[1];
        state.cd_push(name);
    }   
    state.set_cwd(new_cwd);
    DEBUGF ('c', state);
    DEBUGF ('c', words);
}

void fn_echo (inode_state& state, const wordvec& words){
    DEBUGF ('c', state);
    DEBUGF ('c', words);
    cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}


void fn_exit (inode_state& state, const wordvec& words){
    DEBUGF ('c', state);
    DEBUGF ('c', words);
    int exit_code;
    if (words.size() == 1) {
        exit_code = 0;
    } else if (words.size() == 2) {
        stringstream stream (words[1]);
        if (not (stream >> exit_code && stream.eof())) {
            exit_code = 127;
        } 
    } else {
        exit_code = 127;
    }
    if (exit_code > 127 || exit_code < 0) {
        exit_code = 127;
    }
    exit_status::set(exit_code);
    throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words){
    inode_ptr list_dir_pointer;
    vector<string> directory = (state.get_directory());
    if (static_cast<int>(words.size()) == 1) {
        list_dir_pointer = state.get_cwd();
    } else if (static_cast<int>(words.size()) == 2){
        if (words[1] == "/") {
            list_dir_pointer = state.get_root();
        } else if (words[1] == "..") {
            directory.pop_back();
            list_dir_pointer = state.get_cwd()->
                get_contents()->inode_from_name(words[1]);
        } else {
            list_dir_pointer = state.get_cwd()->
                get_contents()->inode_from_name(words[1]);
        }
    } else {
        throw file_error("invalid path name");
    }
    DEBUGF('z', list_dir_pointer);
    DEBUGF('z', directory);
    if (static_cast<int>(directory.size()) == 0) {
        cout << "/";
    } else {
        string name;
        reverse(directory.begin(), directory.end());
        for (int i = 0; i <= static_cast<int>(directory.size()); 
        i++) {
            name = directory.back();
            directory.pop_back();
            cout << "/" << name;
        }
    }
    cout << ":" << endl;
    base_file_ptr dir_contents = list_dir_pointer->get_contents(); 
    dir_contents->print_dir();
    DEBUGF ('c', state);
    DEBUGF ('c', words);
}

void fn_lsr (inode_state& state, const wordvec& words){
    inode_ptr list_dir_pointer;
    vector<string> directory = (state.get_directory());
    inode_ptr curr_dir = state.get_cwd();
    if (static_cast<int>(words.size()) == 1) {
        list_dir_pointer = state.get_cwd();
    } else if (static_cast<int>(words.size()) == 2){
        if (words[1] == "/") {
            state.set_cwd(state.get_root());
            list_dir_pointer = state.get_cwd();
        } else if (words[1] == "..") {
            directory.pop_back();
            list_dir_pointer = state.get_cwd()->
                get_contents()->inode_from_name(words[1]);
        } else {
            list_dir_pointer = state.get_cwd()->
                get_contents()->inode_from_name(words[1]);
        }
    } else {
        throw file_error("invalid path name");
    }
    if (static_cast<int>(directory.size()) == 0) {
        cout << "/";
    } else {
        string name;
        reverse(directory.begin(), directory.end());
        for (int i = 0; i <= static_cast<int>(directory.size()); 
        i++) {
            name = directory.back();
            directory.pop_back();
            cout << "/" << name;
        }
    }
    cout << ":" << endl;
    base_file_ptr dir_contents = list_dir_pointer->get_contents(); 
    dir_contents->print_dir();
    map<string, inode_ptr> directory_contents = 
        dir_contents->get_dirents();
    string directory_name;

    for (auto it = directory_contents.begin(); 
        it != directory_contents.end(); ++it) {
        if ((it->first != ".") && (it->first != "..")) {
            inode_ptr i_ptr = it->second;
            base_file_ptr content = i_ptr->get_contents();
            directory_name = it->first;
            if (content->is_directory()) {
                state.cd_push(directory_name);
                state.set_cwd(i_ptr);
                fn_lsr(state, words);
                state.set_cwd(curr_dir);
            }
        }
    }

    DEBUGF ('c', state);
    DEBUGF ('c', words);
}

void fn_make (inode_state& state, const wordvec& words){
    string filename = words[1];
    string new_filenmame = swap_case(filename);
    inode_ptr curr_dir = state.get_cwd();
    base_file_ptr contents = curr_dir->get_contents();
    inode_ptr new_file = contents->mkfile(new_filenmame);
    wordvec data = words;
    reverse(data.begin(), data.end());
    data.pop_back();
    data.pop_back();
    reverse(data.begin(), data.end());
    new_file->get_contents()->writefile(data);
    DEBUGF ('c', state);
    DEBUGF ('c', words);
}

void fn_mkdir (inode_state& state, const wordvec& words){
    if(words.size() ==1) {
        throw command_error("missing directory name");
    } else if(words.size() > 2) {
        throw command_error("too many words");
    }
    inode_ptr curr = state.get_cwd();
    base_file_ptr content = curr->get_contents();
    string dirname = words[1];
    string new_dirname = swap_case(dirname);
    inode_ptr new_directory = content->mkdir(new_dirname, state);
    DEBUGF ('c', state);
    DEBUGF ('c', words);
}

void fn_prompt (inode_state& state, const wordvec& words){
    string new_prompt = "";
    for (int i = 1; i < static_cast<int>(words.size()); i++) {
        new_prompt += words[i] + " ";
    }
    state.set_prompt(new_prompt);
}

void fn_pwd (inode_state& state, const wordvec& words){
    vector<string> directory = (state.get_directory());
    string new_name;
    if (static_cast<int>(directory.size()) == 0) {
        cout << "/" << endl;
    } else {
        reverse(directory.begin(), directory.end());
        string name;
        for (int i = 0; i <= static_cast<int>(directory.size()); 
        i++) {
            name = directory.back();
            directory.pop_back();
            cout << "/" << name;
        }
        cout << endl;
    }
    DEBUGF ('c', state);
    DEBUGF ('c', words);
}

void fn_rm (inode_state& state, const wordvec& words){
    string filename = words[1];
    string new_filenmame = swap_case(filename);
    inode_ptr curr = state.get_cwd();
    base_file_ptr content = curr->get_contents();
    cout << "Remove: " << filename << endl;
    content->remove(filename);
    DEBUGF ('c', state);
    DEBUGF ('c', words);
}

void fn_rmr (inode_state& state, const wordvec& words){
    inode_ptr list_dir_pointer = state.get_cwd();
    base_file_ptr dir_contents = list_dir_pointer->get_contents(); 
    map<string, inode_ptr> directory_contents = 
        dir_contents->get_dirents();
    inode_ptr i_ptr;
    string filename;
    base_file_ptr content;

    for (auto it = directory_contents.begin(); 
            it != directory_contents.end(); ++it) {
        if ((it->first != ".") && (it->first != "..")) {
            i_ptr = it->second;
            content = i_ptr->get_contents();
            if (content->is_directory()) {               
                state.set_cwd(i_ptr);
                fn_rmr(state, words);
            }
        }
    }

    for (auto it = directory_contents.begin(); 
            it != directory_contents.end(); ++it) {
        if ((it->first != ".") && (it->first != "..")) {
            filename = it->first;
            dir_contents->remove(filename);
        }
    }

    state.set_cwd(state.get_cwd()->get_contents()
        ->inode_from_name(".."));

    DEBUGF ('c', state);
    DEBUGF ('c', words);
}

