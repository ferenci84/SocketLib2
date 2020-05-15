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

int main() {
    cout << sizeof(SOCKET) << endl;
    cout << sizeof(SOCKETLIB_HANDLE) << endl;
    cout << sizeof(void*) << endl;
    cout << sizeof(int) << endl;
    int err;
    SOCKETLIB_HANDLE socket;
    socket = create_server((unsigned char*)"127.0.0.1",6111,10,err);
    if (socket == 0) {
        cout << "no socket" << endl;
    }

    thread client_thread([]{
        this_thread::sleep_for(chrono::milliseconds(350));
        cout << "client: connect to server" << endl;
        int err;
        SOCKETLIB_HANDLE client = create_client((unsigned char*)"127.0.0.1",6111,err);
        if (client == 0) {
            cout << "couldn't connect, error: " << err << endl;
            return;
        }

        this_thread::sleep_for(chrono::milliseconds(300));
        SOCKETLIB_HANDLE recv_buf = create_receive_buffer(client,64);
        char buf[1024];
        if (poll_msg(recv_buf,1000,true)) {
            if (get_msg(recv_buf,(unsigned char*)buf,1024,err)) {
                cout << "message: " << buf << endl;
            } else {
                cout << "error: " << err << endl;
            }
        } else {
            cout << "not received anything" << endl;
        }
        if (poll_msg(recv_buf,1000,true)) {
            if (get_msg(recv_buf,(unsigned char*)buf,1024,err)) {
                cout << "message: " << buf << endl;
            } else {
                cout << "error: " << err << endl;
            }
        } else {
            cout << "not received anything" << endl;
        }
        if (poll_msg(recv_buf,1000,true)) {
            if (get_msg(recv_buf,(unsigned char*)buf,1024,err)) {
                cout << "message: " << buf << endl;
            } else {
                cout << "error: " << err << endl;
            }
        } else {
            cout << "not received anything" << endl;
        }
        if (poll_msg(recv_buf,1000,true)) {
            if (get_msg(recv_buf,(unsigned char*)buf,1024,err)) {
                cout << "message: " << buf << endl;
            } else {
                cout << "error: " << err << endl;
            }
        } else {
            cout << "not received anything" << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(1000));
        cout << "client disconnects" << endl;
        disconnect_socket(client);
        if (poll_msg(recv_buf,1000,true)) {
            if (get_msg(recv_buf,(unsigned char*)buf,1024,err)) {
                cout << "message: " << buf << endl;
            } else {
                cout << "error: " << err << endl;
            }
        } else {
            cout << "not received anything" << endl;
        }
        //delete_receive_buffer(recv_buf);

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

    SOCKETLIB_HANDLE accept_process = create_accept_process(socket);
    SOCKETLIB_HANDLE s_client_socket;
    char client_addr[1024];
    unsigned short client_port;
    int error;
    while (!poll_accept(accept_process,100,true)) {
        cout << "waiting for connection" << endl;
    }
    if (get_accept_result(accept_process,s_client_socket,(unsigned char*)client_addr,1024,client_port,error)) {
        cout << "client connected: " << " socket: " << s_client_socket << endl;
        cout << "address: " << client_addr << endl;
        cout << "port: " << client_port << endl;

        auto queue = create_send_queue(s_client_socket,10,64);
        send_msg(queue,(unsigned char*)"IAMTHESERVER");
        send_msg(queue,(unsigned char*)"IAMTHESERVER");

        this_thread::sleep_for(chrono::milliseconds(5000));

        send_msg(queue,(unsigned char*)"IAMTHESERVER");


        cout << "server disconnects" << endl;
        //disconnect_socket(socket);

        //delete_send_queue(queue);

    }

    //disconnect_socket(socket);
    //delete_accept(accept_process);

    if (client_thread.joinable()) client_thread.join();

    delete_all_socket(0);

    /*this_thread::sleep_for(chrono::milliseconds(1500));

    disconnect_socket_and_forget(s_client_socket);

    disconnect_socket(socket);

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
        disconnect_socket(sock);
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