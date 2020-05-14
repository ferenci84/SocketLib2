//
// Created by ferenci84 on 5/12/2020.
//

#include "socketlib2.h"
#include "socketlib2_send.h"
#include <iostream>
#include <future>
#include <list>

using namespace std;

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall send(SOCKETLIB_HANDLE sock, unsigned char* msg, int len) {
    if (len <= 0) {
        len = (int)strlen((char*)msg)+1;
    }
    auto task = new send_task(sock,(char*)msg,len);
    return (SOCKETLIB_HANDLE)task;
}

SOCKETLIB2_API bool __stdcall poll_send(SOCKETLIB_HANDLE task_handle, int wait_for_ms) {
    auto task = (send_task*)task_handle;
    future<send_task_result>* task_future = task->get_task();
    return (task_future->wait_for(chrono::milliseconds(wait_for_ms)) == future_status::ready);
}

SOCKETLIB2_API bool __stdcall finish_send(SOCKETLIB_HANDLE task_handle, int& error) {
    auto task = (send_task*)task_handle;
    future<send_task_result>* task_future = task->get_task();
    auto result = task_future->get();
    delete task;
    error = result.error;
    return result.success;
}

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_send_queue(SOCKETLIB_HANDLE sock, unsigned int max_queue_size, unsigned int buffer_size) {
    return (SOCKETLIB_HANDLE)(new send_queue(sock, max_queue_size, buffer_size));
}

SOCKETLIB2_API void __stdcall send_msg(SOCKETLIB_HANDLE queue_handle, unsigned char* msg) {
    auto queue = (send_queue*)queue_handle;
    queue->send_msg(string((char*)msg));
}

SOCKETLIB2_API int __stdcall get_queue_error(SOCKETLIB_HANDLE queue_handle) {
    auto queue = (send_queue*)queue_handle;
    return queue->get_last_error();
}

SOCKETLIB2_API void __stdcall delete_send_queue(SOCKETLIB_HANDLE queue_handle) {
    auto queue = (send_queue*)queue_handle;
    delete queue;
}
