//
// Created by ferenci84 on 5/12/2020.
//

#include "socketlib2.h"
#include "socketlib2_accept.h"
#include <iostream>
#include <future>
#include <list>

using namespace std;

// INTERNAL
SOCKETLIB_HANDLE accept_sync(SOCKET sock, struct sockaddr_in& connectSAddr, int& error) {
    struct sockaddr_in connectSAddr1;
    int addrLen = sizeof(connectSAddr1);
    cout << "accept connection" << endl;
    SOCKET connsock = accept(sock, (struct sockaddr *)&connectSAddr1, &addrLen);
    if (connsock==INVALID_SOCKET) {
        error = WSAGetLastError();
        return 0;
    }

    connectSAddr = connectSAddr1;
    return connsock;
}

// TODO: add a class to manage references using set
// It will check if the reference exists, and will delete if not deleted explicitly

// INTERNAL
struct accept_task_result {
    SOCKETLIB_HANDLE client_socket;
    struct sockaddr_in connectSAddr;
    int error;
};

// INTERNAL
class accept_task {
private:
    future<accept_task_result> task;
public:
    explicit accept_task(SOCKETLIB_HANDLE sock) {
        task = async(launch::async, [&](SOCKETLIB_HANDLE sock) {
            accept_task_result result{};
            result.client_socket = accept_sync(sock,result.connectSAddr,result.error);
            return result;
        },sock);
    }
    future<accept_task_result>* get_task() {
        return &task;
    }
};

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall accept(SOCKETLIB_HANDLE sock) {
    auto task = new accept_task(sock);
    return (SOCKETLIB_HANDLE)task;
}

SOCKETLIB2_API bool __stdcall poll_accept(SOCKETLIB_HANDLE task_handle, int wait_for_ms, SOCKETLIB_HANDLE& client_socket, unsigned char* client_addr_buf, unsigned int client_addr_buf_size, unsigned short& client_port, int& error) {
    auto task = (accept_task*)task_handle;
    future<accept_task_result>* task_future = task->get_task();
    if (task_future->wait_for(chrono::milliseconds(wait_for_ms)) == future_status::ready) {
        auto result = task_future->get();
        client_socket = result.client_socket;
        error = result.error;
        if (client_socket != 0) {
            auto sin = &result.connectSAddr;
            client_port = sin->sin_port;
            inet_ntop(sin->sin_family, &(sin->sin_addr), (char *) client_addr_buf, 64);
        }
        delete task;
        return true;
    } else {
        return false;
    }
}

