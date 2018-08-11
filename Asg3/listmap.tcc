// $Id: listmap.tcc,v 1.11 2018-01-25 14:19:14-08 - - $
// Connor Thurmond (cthurmon) Rami Alrwais (raalrwai)

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
    DEBUGF ('l', reinterpret_cast<const void*> (this));
    while (begin() != end()) {
        iterator erase_elem = begin();
        erase(erase_elem);
    }
}


//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
    node* insert_node = end().where;
    iterator head = begin();
    iterator tail = end();
    if (begin() == end()) {
        insert_node = new node(tail.where, head.where, pair);
        head.where->next = insert_node;
        tail.where->prev = insert_node;
    } else {
        node* curr = head.where;
        while (curr != end().where and 
               less(curr->value.first, pair.first)) {
            curr = curr->next;
        }
        if (curr == end().where) {
            insert_node = new node(tail.where, tail.where->prev, pair);
            tail.where->prev->next = insert_node;
            tail.where->prev = insert_node;
        } else if (curr == begin().where) {
            if (curr->value.first == pair.first) {
                curr->value.second = pair.second;
            } else {
                insert_node = new node(curr, tail.where, pair);
                curr->prev = insert_node;
                tail.where->next = insert_node;
            }
        } else {
            if (curr->value.first == pair.first) {
                curr->value.second = pair.second;
            } else {
                insert_node = new node(curr, curr->prev, pair);
                curr->prev->next = insert_node;
                curr->prev = insert_node;
            }
        }
    }
    cout << pair.first << " = " << pair.second << endl;
    return insert_node;
}

//
// listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) {
    node* head = anchor()->next;
    node* tail = anchor();
    node* curr = head;
    iterator curr_itor = begin();
    while (curr != tail) {
        if (curr->value.first == that) {
            return curr_itor;
        }
        curr = curr->next;
        ++curr_itor;
    }
    return curr_itor;
    DEBUGF ('l', that);
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
    if (position.where) {
        position.where->prev->next = position.where->next;
        position.where->next->prev = position.where->prev;
        delete position.where;
    }
    DEBUGF ('l', &*position);
    return iterator();
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
    DEBUGF ('l', where);
    return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
    DEBUGF ('l', where);
    return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
    DEBUGF ('l', where);
    where = where->next;
    return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
    DEBUGF ('l', where);
    where = where->prev;
    return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
                (const iterator& that) const {
    return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
                (const iterator& that) const {
    return this->where != that.where;
}

