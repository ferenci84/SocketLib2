//
// Created by ferenci84 on 5/12/2020.
//


#include "socketlib2.h"
#include "socketlib2_receive.h"
#include "socketlib2_manage_resources.h"
#include <iostream>
#include <future>
#include <list>

using namespace std;

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_receive_buffer(SOCKETLIB_HANDLE sock, unsigned int size) {
    auto handle = retrieve_receive_buffer(sock);
    if (handle == 0){
        handle = (SOCKETLIB_HANDLE)(new msg_buffer(sock,size));
        add_receive_buffer(sock, handle);
    }
    return handle;
}

SOCKETLIB2_API void __stdcall delete_receive_buffer(SOCKETLIB_HANDLE buffer) {
    if (!receive_buffer_exists(buffer)) return;
    remove_receive_buffer(buffer);
    auto recv_buf = (msg_buffer*)buffer;
    delete recv_buf;
}

SOCKETLIB2_API bool __stdcall poll_msg(SOCKETLIB_HANDLE buffer, int wait_ms, bool restart) {
    if (!receive_buffer_exists(buffer)) {
        return true;
    }
    auto recv_buf = (msg_buffer*)buffer;
    return recv_buf->poll_msg(wait_ms,restart);
}


SOCKETLIB2_API bool __stdcall get_msg(SOCKETLIB_HANDLE buffer, unsigned char* msg_buf, int buf_size, int& err) {
    if (!receive_buffer_exists(buffer)) {
        err = 12000;
        return false;
    }
    auto recv_buf = (msg_buffer*)buffer;
    string msg;
    if (recv_buf->get_last_result(msg,err)){
        if (buf_size < msg.size()+1) {
            cout << "buffer size is too small" << endl;
            return false;
        } else {
            strcpy((char*)msg_buf,msg.c_str());
            return true;
        }
    } else {
        if (buf_size > 0) *msg_buf = 0;
        return false;
    }
}