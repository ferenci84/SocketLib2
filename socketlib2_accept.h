//
// Created by ferenci84 on 5/14/2020.
//

#ifndef SOCKETLIB2_SOCKETLIB2_ACCEPT_H
#define SOCKETLIB2_SOCKETLIB2_ACCEPT_H

#include "socketlib2.h"
#include <future>
#include <string>
#include <iostream>

using namespace std;

bool accept(SOCKET sock, SOCKET& connsock, string& addr, unsigned short& port, int& error) {
    struct sockaddr_in connectSAddr;
    int addrLen = sizeof(connectSAddr);
    cout << "accept connection" << endl;
    connsock = accept(sock, (struct sockaddr *)&connectSAddr, &addrLen);
    if (connsock==INVALID_SOCKET) {
        error = WSAGetLastError();
        return false;
    }
    port = connectSAddr.sin_port;
    char addr_buf[64];
    inet_ntop(connectSAddr.sin_family, &(connectSAddr.sin_addr), (char *) addr_buf, 64);
    addr = string(addr_buf);
    cout << "addr: " << addr << endl;
    return true;
}

struct accept_result {
    bool success{};
    SOCKET sock{};
    string addr;
    unsigned short port{};
    int error{};
};

class accept_connections {
    SOCKET sock;
    future<struct accept_result> accept_future;
    bool future_in_progress;
    bool connected;
    struct accept_result res;
public:
    accept_connections(SOCKET sock): sock(sock), connected(true), future_in_progress(false) {}
    bool is_connected() {return connected;}
    ~accept_connections() {
        if (future_in_progress) {
            accept_future.get();
        }
    }
private:
    struct accept_result accept() {
        struct accept_result res;
        res.success = ::accept(sock,res.sock,res.addr,res.port,res.error);
        return res;
    }

public:
    bool poll_accept(int wait_for, bool restart = true) {
        if (!this->is_connected()) return false;
        bool start_task = false;
        if (!future_in_progress) {
            cout << "starting accept task" << endl;
            accept_future = async(launch::async, &accept_connections::accept, this);
            future_in_progress = true;
        }
        if (accept_future.wait_for(std::chrono::milliseconds(wait_for)) == future_status::ready) {
            res = accept_future.get();
            if (restart) {
                cout << "restarting accept task" << endl;
                accept_future = async(launch::async, &accept_connections::accept, this);
            } else {
                future_in_progress = false;
            }
            return true;
        }
        return false;
    }
    bool get_last_result(SOCKET& client_sock, string& addr, unsigned short& port, int& error) {
        if (res.success) {
            client_sock = res.sock;
            addr = res.addr;
            port = res.port;
        } else {
            error = res.error;
        }
        return res.success;
    }
};


#endif //SOCKETLIB2_SOCKETLIB2_ACCEPT_H
