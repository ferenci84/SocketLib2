
#include "socketlib2.h"
#include <thread>
#include <iostream>

using namespace std;



int main() {

    string addr = "127.0.0.1";
    unsigned short port = 5321;

    auto th = new thread([addr,port] {
        int err;
        SOCKETLIB_HANDLE sock = open_socket_dgam(err);
        if (sock == 0) {
            cout << "Error: " << err << endl;
        }

        const char* caddr = addr.c_str();

        if (!::bind(sock,(unsigned char*)caddr,port,err)) {
            cout << "Client Bind Error: " << err << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
        char buf[512];
        int bytes = receive_dgram_sync(sock,(unsigned char*)caddr,port,(unsigned char*)buf,512,err);
        if (bytes == -1) {
            cout << "received bytes: " << bytes << endl;
            cout << "Error receiving: " << err << endl;
        } else {
            cout << " msg:" << buf << endl;
        }
    });

    int err;
    SOCKETLIB_HANDLE sock = open_socket_dgam(err);
    if (sock == 0) {
        cout << "Error: " << err << endl;
    }
    const char* caddr = addr.c_str();

    if (!::bind(sock,(unsigned char*)caddr,port,err)) {
        cout << "Bind Error: " << err << endl;
    }

    this_thread::sleep_for(chrono::milliseconds(1000));

    string msg = "THISISASMALLMESSAGE";
    const char* cmsg = msg.c_str();
    int bytes = send_dgram(sock,(unsigned char*)caddr,port,(unsigned char*)cmsg,err);
    if (bytes == -1) {
        cout << "Error: " << err << endl;
    } else {
        cout << "sent bytes " << bytes << " of " << msg.size()+1 << endl;
    }

    th->join();
    delete th;

    return 0;
}