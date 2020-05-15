//
// Created by ferenci84 on 5/15/2020.
//

#include "socketlib2_manage_resources.h"

#include <map>
#include <set>
#include <iostream>

using namespace std;

set<SOCKETLIB_HANDLE> allocated_sockets;
set<SOCKETLIB_HANDLE> allocated_accept_process;
set<SOCKETLIB_HANDLE> allocated_send_queue;
set<SOCKETLIB_HANDLE> allocated_receive_buffer;
map<SOCKETLIB_HANDLE,SOCKETLIB_HANDLE> map_accept_process;
map<SOCKETLIB_HANDLE,SOCKETLIB_HANDLE> map_send_queue;
map<SOCKETLIB_HANDLE,SOCKETLIB_HANDLE> map_receive_buffer;


void clean_up() {
    cout << "cleaning up" << endl;
    set<SOCKETLIB_HANDLE> copy = allocated_sockets;
    for (SOCKETLIB_HANDLE socket: copy) {
        disconnect_socket(socket);
    }
}

void add_socket(SOCKETLIB_HANDLE socket) {
    cout << "adding socket " << socket << endl;
    allocated_sockets.insert(socket);
}

void remove_socket(SOCKETLIB_HANDLE socket) {
    if (allocated_sockets.count(socket) != 0) {
        cout << "removing socket " << socket << endl;
        allocated_sockets.erase(socket);
    }
}

bool socket_exists(SOCKETLIB_HANDLE socket) {
    return allocated_sockets.count(socket) != 0;
}

void add_accept_process(SOCKETLIB_HANDLE socket, SOCKETLIB_HANDLE accept_process) {
    cout << "adding accept process: " << socket << " " << accept_process << endl;
    allocated_accept_process.insert(accept_process);
    map_accept_process[socket] = accept_process;
}

SOCKETLIB_HANDLE retrieve_accept_process(SOCKETLIB_HANDLE socket) {
    if (map_accept_process.count(socket) != 0) {
        SOCKETLIB_HANDLE handle = map_accept_process[socket];
        if (allocated_accept_process.count(handle)) {
            cout << "retrieving accept process: " << handle << endl;
            return handle;
        }
    }
    return 0;
}

void remove_accept_process(SOCKETLIB_HANDLE handle) {
    if (allocated_accept_process.count(handle) != 0) {
        cout << "remove accept process: " << handle << endl;
        allocated_accept_process.erase(handle);
    }
}

bool accept_process_exists(SOCKETLIB_HANDLE handle) {
    return allocated_accept_process.count(handle) != 0;
}

void delete_owned_accept_process(SOCKETLIB_HANDLE socket) {
    cout << "delete any owned accept process: " << socket << endl;
    SOCKETLIB_HANDLE handle = retrieve_accept_process(socket);
    if (handle != 0) {

        delete_accept(handle);
    }
}

void add_send_queue(SOCKETLIB_HANDLE socket, SOCKETLIB_HANDLE send_queue) {
    allocated_send_queue.insert(send_queue);
    map_send_queue[socket] = send_queue;
}


SOCKETLIB_HANDLE retrieve_send_queue(SOCKETLIB_HANDLE socket) {
    if (map_send_queue.count(socket) != 0) {
        SOCKETLIB_HANDLE handle = map_send_queue[socket];
        if (allocated_send_queue.count(handle)) {
            return handle;
        }
    }
    return 0;
}


void remove_send_queue(SOCKETLIB_HANDLE handle) {
    if (allocated_send_queue.count(handle) != 0) {
        allocated_send_queue.erase(handle);
    }
}

bool send_queue_exists(SOCKETLIB_HANDLE handle) {
    return allocated_send_queue.count(handle) != 0;
}

void delete_owned_send_queue(SOCKETLIB_HANDLE socket){
    SOCKETLIB_HANDLE handle = retrieve_send_queue(socket);
    if (handle != 0) {
        delete_send_queue(handle);
    }
}

void add_receive_buffer(SOCKETLIB_HANDLE socket, SOCKETLIB_HANDLE receive_buffer) {
    allocated_receive_buffer.insert(receive_buffer);
    map_receive_buffer[socket] = receive_buffer;
}



SOCKETLIB_HANDLE retrieve_receive_buffer(SOCKETLIB_HANDLE socket) {
    if (map_receive_buffer.count(socket) != 0) {
        SOCKETLIB_HANDLE handle = map_receive_buffer[socket];
        if (allocated_receive_buffer.count(handle)) {
            return handle;
        }
    }
    return 0;
}


void remove_receive_buffer(SOCKETLIB_HANDLE handle) {
    if (allocated_receive_buffer.count(handle) != 0) {
        allocated_receive_buffer.erase(handle);
    }
}

bool receive_buffer_exists(SOCKETLIB_HANDLE handle) {
    return allocated_receive_buffer.count(handle) != 0;
}

void delete_owned_receive_buffer(SOCKETLIB_HANDLE socket) {
    SOCKETLIB_HANDLE handle = retrieve_send_queue(socket);
    if (handle != 0) {
        delete_receive_buffer(handle);
    }
}



