//
// Created by ferenci84 on 5/14/2020.
//

#ifndef SOCKETLIB2_SOCKETLIB_SERVER_H
#define SOCKETLIB2_SOCKETLIB_SERVER_H

#include "socketlib2.h"
#include "socketlib2_accept.h"
#include "socketlib2_receive.h"
#include "socketlib2_send.h"

SOCKET create_server(const string& addr, int port, int queue_size, int& err) {
    SOCKET sock = open_socket(err);
    if (sock == 0) {
        cout << "Failed server socket() call, Error: " << err << endl;
        return 0;
    }

    if (!bind(sock,(unsigned char*)addr.c_str(),port,err)) {
        cout << "Failed to bind socket, Error: " << err << endl;
        disconnect_socket_and_forget(sock);
        return 0;
    }

    if (!listen(sock, queue_size,err)) {
        cout << "Failed to listen socket, Error: " << err << endl;
        disconnect_socket_and_forget(sock);
        return 0;
    }
    cout << "Opened socket: " << sock << endl;
    return sock;
}

class socket_server {
    SOCKET sock;
    bool connected;
    accept_connections accept;
    void open(const string& addr, int port, int queue_size, int& err) {
        sock = create_server(addr,port,queue_size,err);
    }
public:
    socket_server(const string& addr, int port, int queue_size, int& err) : sock(0), connected(false) {
        this->open(addr,port,queue_size,err);
        if (sock != 0) {
            connected = true;
            accept = new accept_connections(sock);
        }
    }
    ~socket_server() {
        disconnect_socket_and_forget(sock);
        delete accept;
    }
};

#endif //SOCKETLIB2_SOCKETLIB_SERVER_H
