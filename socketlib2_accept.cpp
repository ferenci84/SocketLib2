//
// Created by ferenci84 on 5/12/2020.
//

#include "socketlib2.h"
#include "socketlib2_accept.h"
#include "socketlib2_manage_resources.h"
#include <iostream>
#include <future>
#include <list>

using namespace std;

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_accept_process(SOCKETLIB_HANDLE socket) {
    auto handle = retrieve_accept_process(socket);
    if (handle == 0){
        handle = (SOCKETLIB_HANDLE)(new accept_connections(socket));
        add_accept_process(socket, handle);
    }
    return handle;
}

SOCKETLIB2_API void __stdcall delete_accept(SOCKETLIB_HANDLE accept_handle) {
    if (!accept_process_exists(accept_handle)) return;
    remove_accept_process(accept_handle);
    auto accept = (accept_connections*)accept_handle;
    delete accept;
}
SOCKETLIB2_API bool __stdcall poll_accept(SOCKETLIB_HANDLE accept_handle, int wait_for, bool restart) {
    auto accept = (accept_connections*)accept_handle;
    return accept->poll_accept(wait_for, restart);
}

SOCKETLIB2_API bool __stdcall get_accept_result(SOCKETLIB_HANDLE accept_handle, SOCKETLIB_HANDLE& client_sock, unsigned char* addr_buf, unsigned int addr_buf_size, unsigned short& port, int& error) {
    auto accept = (accept_connections*)accept_handle;
    string addr;
    bool succ = accept->get_last_result(client_sock, addr, port, error);
    if (addr.size() <= addr_buf_size+1){
        strcpy((char*)addr_buf,addr.c_str());
    } else {
        if (addr_buf_size > 0) *((char*)addr_buf) = 0;
    }
    return succ;
}
