// $Id: file_sys.cpp,v 1.6 2018-06-27 14:44:57-07 - - $
// Connor Thurmond (cthurmon) Rami Alrwais (raalrwai)

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <iomanip>
#include <algorithm>
using namespace std;

#include "debug.h"
#include "file_sys.h"

int inode::next_inode_nr {1};

struct file_type_hash {
    size_t operator() (file_type type) const {
        return static_cast<size_t> (type);
    }
};

ostream& operator<< (ostream& out, file_type type) {
    static unordered_map<file_type,string,file_type_hash> hash {
        {file_type::PLAIN_TYPE, "PLAIN_TYPE"},
        {file_type::DIRECTORY_TYPE, "DIRECTORY_TYPE"},
    };
    return out << hash[type];
}

inode_state::inode_state() {
    inode* root_node = new inode(file_type::DIRECTORY_TYPE);
    root = make_shared<inode>(*root_node);
    cwd = root;
    cwd->get_contents()->add_map_pair(".", cwd);
    cwd->get_contents()->add_map_pair("..", cwd);
    DEBUGF ('i', "root = " << root << ", cwd = " << cwd
        << ", prompt = \"" << prompt() << "\"");
}


const string& inode_state::prompt() const { return prompt_; }

const string& inode_state::set_prompt(const string& new_prompt) {
    prompt_ = new_prompt;
    return prompt_;
}

inode_ptr inode_state::get_cwd() {
    return cwd;
}

void inode_state::set_cwd(inode_ptr curr_dir) {
    cwd = curr_dir;
}

inode_ptr& inode_state::get_root() {
    return root;
}

vector<string> inode_state::get_directory() {
    return directory;
}

void inode_state::cd_push(string& dirname) {
    directory.push_back(dirname);
}

void inode_state::cd_pop() {
    directory.pop_back();
}

ostream& operator<< (ostream& out, const inode_state& state) {
    out << "inode_state: root = " << state.root
        << ", cwd = " << state.cwd;
    return out;
}

inode::inode(file_type type): inode_nr (next_inode_nr++) {
    switch (type) {
        case file_type::PLAIN_TYPE:
            contents = make_shared<plain_file>();
            break;
        case file_type::DIRECTORY_TYPE:
            contents = make_shared<directory>();
            break;
    }
    DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

int inode::get_inode_nr() const {
    DEBUGF ('i', "inode = " << inode_nr);
    return inode_nr;
}

base_file_ptr inode::get_contents() {
    return contents;
}


file_error::file_error (const string& what):
    runtime_error (what) {
    }

size_t plain_file::size() const {
    size_t size = 0;
    if (data.size() == 0) {
        size = 0;
    } else {
        size += data.size() - static_cast<size_t>(1);
        string curr_string;
        for (auto it = data.begin(); it != data.end(); it++) {
            size += it->size();
        }

    }
    DEBUGF ('i', "size = " << size);
    return size;
}

const wordvec& plain_file::readfile() const {
    DEBUGF ('i', data);
    return data;
}

void plain_file::writefile (const wordvec& words) {
    data = words;
    DEBUGF ('i', words);
}

inode_ptr plain_file::inode_from_name(const string& value){
    throw file_error("file does not exist");
    DEBUGF ('i', "size = " << value);
}

void plain_file::remove (const string&) {
    throw file_error ("is a plain file");
}

inode_ptr plain_file::mkdir (const string&, inode_state& state) {
    throw file_error ("is a plain file");
    DEBUGF ('i', state);
}

inode_ptr plain_file::mkfile (const string&) {
    throw file_error ("is a plain file");
}

void plain_file::print_dir () {
    throw file_error ("is a plain file");
}

void plain_file::recursive_print_dir () {
    throw file_error ("is a plain file");
}

map<string, inode_ptr> plain_file::get_dirents () {
    throw file_error ("is a plain file");
}


void plain_file::add_map_pair(const string& dirname, inode_ptr i_ptr) {
    throw file_error ("is a plain file");
    DEBUGF ('i', dirname);
    DEBUGF ('i', i_ptr);
}

bool plain_file::is_directory() {
    return false;
}



size_t directory::size() const {
    size_t size = dirents.size();
    DEBUGF ('i', "size = " << size);
    return size;
}

const wordvec& directory::readfile() const {
    throw file_error ("is a directory");
}
inode_ptr directory::inode_from_name(const string& value){
    inode_ptr val = dirents[value];
    if (val) {
        return val;
    } else {
        return NULL;
    }  
}

void directory::writefile (const wordvec&) {
    throw file_error ("is a directory");
}

void directory::remove (const string& filename) {
    dirents.erase(filename);
    DEBUGF ('i', filename);
}

inode_ptr directory::mkdir (const string& dirname, inode_state& state) {
    inode* new_directory = new inode(file_type::DIRECTORY_TYPE);
    inode_ptr new_directory_ptr = make_shared<inode>(*new_directory);
    dirents[dirname] = new_directory_ptr;
    new_directory_ptr->get_contents()->
        add_map_pair(".", new_directory_ptr);
    new_directory_ptr->get_contents()->
        add_map_pair("..", state.get_cwd());
    return new_directory_ptr;
    DEBUGF ('i', dirname);
}

void directory::add_map_pair(const string& dirname, inode_ptr i_ptr) {
    dirents[dirname] = i_ptr;
}

inode_ptr directory::mkfile (const string& filename) {
    inode* new_file = new inode(file_type::PLAIN_TYPE);
    inode_ptr new_file_ptr = make_shared<inode>(*new_file);
    dirents[filename] = new_file_ptr;
    return new_file_ptr;
    DEBUGF ('i', filename);
}


void directory::print_dir () {
    string dirname;
    string new_dirname;
    for (auto iter = dirents.begin(); iter != dirents.end(); iter++) {
        inode_ptr i_ptr = iter->second;
        if (i_ptr) {
            dirname = iter->first;
            new_dirname = swap_case(dirname);
            base_file_ptr val = i_ptr->get_contents();
            size_t size = (val->size());
            int i_nr = i_ptr->get_inode_nr();
            cout << setw(6) << i_nr << "  " << setw(6)
                << size << "  " << new_dirname;
            if ((iter->first != ".") && (iter->first != "..")) {
                if (iter->second->get_contents()->is_directory()) {
                    cout << "/";
                }
            }
            cout << endl; 
        } else {
            break;
        }
    }
}

void directory::recursive_print_dir () {
    string dirname;
    string tee = "T";
    for (auto it = dirents.begin(); it != dirents.end(); ++it) {
        dirname = it->first;
        if (dirname.compare(0, tee.size(), tee) == 0) {
            dirname.erase(0, 1);
            dirname.insert(0, "t");
        }
        inode_ptr i_ptr = it->second;
        base_file_ptr val = i_ptr->get_contents();
        size_t size = (val->size());
        int i_nr = i_ptr->get_inode_nr();
        cout << setw(6) << i_nr << "  " << setw(6)
            << size << "  " << dirname;
        if ((it->first != ".") && (it->first != "..")) {
            if (it->second->get_contents()->is_directory()) {
                cout << "/";
            }
        }
        cout << endl; 
    }
    for (auto it = dirents.begin(); it != dirents.end(); ++it) {
        if ((it->first != ".") && (it->first != "..")) {
            inode_ptr i_ptr = it->second;
            base_file_ptr content = i_ptr->get_contents();
            if (content->is_directory()) {
                content->recursive_print_dir();
            }
        }
    }
}

map<string, inode_ptr> directory::get_dirents () {
    return dirents;
}

bool directory::is_directory() {
    return true;
}
