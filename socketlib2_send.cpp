//
// Created by ferenci84 on 5/12/2020.
//

#include "socketlib2.h"
#include "socketlib2_send.h"
#include "socketlib2_manage_resources.h"
#include <iostream>
#include <future>
#include <list>

using namespace std;

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_send_queue(SOCKETLIB_HANDLE sock, unsigned int max_queue_size, unsigned int buffer_size) {
    auto handle = retrieve_send_queue(sock);
    if (handle == 0){
        handle = (SOCKETLIB_HANDLE)(new send_queue(sock, max_queue_size, buffer_size));
        add_send_queue(sock, handle);
    }
    return handle;
}


SOCKETLIB2_API void __stdcall delete_send_queue(SOCKETLIB_HANDLE queue_handle) {
    if (!send_queue_exists(queue_handle)) return;
    remove_send_queue(queue_handle);
    auto queue = (send_queue*)queue_handle;
    delete queue;
}

SOCKETLIB2_API void __stdcall send_msg(SOCKETLIB_HANDLE queue_handle, unsigned char* msg) {
    auto queue = (send_queue*)queue_handle;
    queue->send_msg(string((char*)msg));
}

SOCKETLIB2_API int __stdcall get_queue_error(SOCKETLIB_HANDLE queue_handle) {
    auto queue = (send_queue*)queue_handle;
    return queue->get_last_error();
}

