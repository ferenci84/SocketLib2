//
// Created by ferenci84 on 5/11/2020.
//

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include "socketlib2.h"
#include <thread>

using namespace std;

SOCKETLIB_HANDLE open_socket(const string& addr, int port, int queue_size);
SOCKETLIB_HANDLE connect_to_server(const string& addr, int port);
bool poll_send(SOCKETLIB_HANDLE send_task, int wait, bool& success);

class wsa {
public:
    wsa() {
        int err;
        wsa_startup(err);
    }
    ~wsa() {
        int err;
        wsa_cleanup(err);
    }
};

int main() {
    cout << sizeof(SOCKET) << endl;
    cout << sizeof(SOCKETLIB_HANDLE) << endl;
    cout << sizeof(void*) << endl;
    cout << sizeof(int) << endl;
    wsa wsa;
    int err;
    SOCKETLIB_HANDLE socket;
    socket = open_socket("127.0.0.1",6111,10);
    if (socket == 0) {
        cout << "no socket" << endl;
    }

    thread client_thread([]{
        this_thread::sleep_for(chrono::milliseconds(350));
        cout << "client: connect to server" << endl;
        SOCKETLIB_HANDLE client = connect_to_server("127.0.0.1",6111);
        this_thread::sleep_for(chrono::milliseconds(300));
        SOCKETLIB_HANDLE recv_buf = create_receive_buffer(client,64);
        char buf[1024];
        int err;
        if (poll_msg(recv_buf,1000,(unsigned char*)buf,1024,err)) {
            cout << "message: " << buf << endl;
        } else {
            cout << "not received anything" << endl;
        }
        if (poll_msg(recv_buf,1000,(unsigned char*)buf,1024,err)) {
            cout << "message: " << buf << endl;
        } else {
            cout << "not received anything" << endl;
        }
        if (poll_msg(recv_buf,1000,(unsigned char*)buf,1024,err)) {
            cout << "message: " << buf << endl;
        } else {
            cout << "not received anything" << endl;
        }
        if (poll_msg(recv_buf,1000,(unsigned char*)buf,1024,err)) {
            cout << "message: " << buf << endl;
        } else {
            cout << "not received anything" << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(1000));
        cout << "client disconnects" << endl;
        disconnect_socket_and_forget(client);
        if (poll_msg(recv_buf,100,(unsigned char*)buf,1024,err)) {
            cout << "message: " << buf << endl;
        }
        delete_receive_buffer(recv_buf);

        /*char buf[1024];
        SOCKETLIB_HANDLE receive_task = receive(client,(unsigned char*)buf,1024);
        int bytes;
        int error;
        while(!poll_receive(receive_task,100)){}
        bytes = finish_receive(receive_task,error);
        if (bytes >= 1) {
            string msg(buf,buf+bytes);
            cout << "received " << bytes << " bytes: " << buf << endl;
        } else {
            cout << "failed to receive, error: " << error << endl;
        }*/

    });

    SOCKETLIB_HANDLE accept_task = accept(socket);
    SOCKETLIB_HANDLE s_client_socket;
    char client_addr[1024];
    unsigned short client_port;
    int error;
    while (!poll_accept(accept_task,100,s_client_socket,(unsigned char*)&client_addr, 1024,client_port,error)) {
        cout << "waiting for connection" << endl;
    }
    if (s_client_socket != 0) {
        cout << "client connected: " << " socket: " << s_client_socket << endl;
        cout << "address: " << client_addr << endl;
        cout << "port: " << client_port << endl;

        auto queue = create_send_queue(s_client_socket,10,64);
        send_msg(queue,(unsigned char*)"IAMTHESERVER");
        send_msg(queue,(unsigned char*)"IAMTHESERVER");
        delete_send_queue(queue);

    }

    this_thread::sleep_for(chrono::milliseconds(5000));
    cout << "server disconnects" << endl;
    disconnect_socket_and_forget(socket);
    if (client_thread.joinable()) client_thread.join();

    /*this_thread::sleep_for(chrono::milliseconds(1500));

    disconnect_socket_and_forget(s_client_socket);

    disconnect_socket_and_forget(socket);

    if (client_thread.joinable()) client_thread.join();*/


}

/*
bool poll_send(SOCKETLIB_HANDLE send_task, int wait, bool& success) {
    int err;
    if (poll_send(send_task,wait)) {
        if (!finish_send(send_task,err)) {
            cout << "Couldn't send message: " << err << endl;
        } else {
            cout << "Message sent" << endl;
        }
        return true;
    }
    return false;
}
 */

SOCKETLIB_HANDLE open_socket(const string& addr, int port, int queue_size) {
    int err;
    SOCKETLIB_HANDLE sock = open_socket(err);
    if (sock == 0) {
        cout << "Failed server socket() call, Error: " << err << endl;
        return 0;
    }

    if (!bind(sock,(unsigned char*)addr.c_str(),port,err)) {
        cout << "Failed to bind socket, Error: " << err << endl;
        return 0;
    }

    if (!listen(sock, queue_size,err)) {
        cout << "Failed to bind socket, Error: " << err << endl;
        disconnect_socket_and_forget(sock);
        return 0;
    }
    cout << "Opened socket: " << sock << endl;
    return sock;
}

SOCKETLIB_HANDLE connect_to_server(const string& addr, int port) {
    int err;
    SOCKETLIB_HANDLE sock = open_socket(err);
    if (sock == 0) {
        cout << "Failed client socket() call, Error: " << err << endl;
        return 0;
    }

    if (!connect(sock,(unsigned char*)addr.c_str(),port,err)) {
        cout << "Failed to connect: " << err << endl;
        return 0;
    }

    return sock;
}