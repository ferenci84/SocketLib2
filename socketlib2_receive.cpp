//
// Created by ferenci84 on 5/12/2020.
//


#include "socketlib2.h"
#include "socketlib2_receive.h"
#include <iostream>
#include <future>
#include <list>

using namespace std;

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall receive(SOCKETLIB_HANDLE sock, unsigned char* buf, int buffer_size) {
    auto task = new receive_task(sock,(char*)buf,buffer_size);
    return (SOCKETLIB_HANDLE)task;
}

SOCKETLIB2_API bool __stdcall poll_receive(SOCKETLIB_HANDLE task_handle, int wait_for_ms) {
    auto task = (receive_task*)task_handle;
    future<receive_task_result>* task_future = task->get_task();
    return task_future->wait_for(chrono::milliseconds(wait_for_ms)) == future_status::ready;
}

SOCKETLIB2_API int __stdcall finish_receive(SOCKETLIB_HANDLE task_handle, int& error) {
    auto task = (receive_task*)task_handle;
    future<receive_task_result>* task_future = task->get_task();
    auto result = task_future->get();
    delete task;
    error = result.error;
    return result.bytes_received;
}

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_receive_buffer(SOCKETLIB_HANDLE sock, unsigned int size) {
    auto recv_buf = new msg_buffer(sock,size);
    return (SOCKETLIB_HANDLE)recv_buf;
}

SOCKETLIB2_API void __stdcall delete_receive_buffer(SOCKETLIB_HANDLE buffer) {
    auto recv_buf = (msg_buffer*)buffer;
    delete recv_buf;
}

SOCKETLIB2_API bool __stdcall poll_msg(SOCKETLIB_HANDLE buffer, int wait_ms, unsigned char* msg_buf, int buf_size, int& err) {
    auto recv_buf = (msg_buffer*)buffer;
    string msg;
    if (recv_buf->poll_msg(msg,wait_ms)) {
        if (buf_size < msg.size()+1) {
            cout << "buffer size is too small" << endl;
            return false;
        } else {
            strcpy((char*)msg_buf,msg.c_str());
            return true;
        }
    } else {
        return false;
    }
}