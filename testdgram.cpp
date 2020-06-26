
#include "socketlib2.h"
#include <thread>
#include <iostream>

using namespace std;



int main() {

    string addr = "127.0.0.1";
    unsigned short port = 5321;

    auto process = [port] {
        int err;
        SOCKETLIB_HANDLE sock = open_socket_dgam_for_receive(err);
        if (sock == 0) {
            cout << "Error: " << err << endl;
        }

        string addr = "0.0.0.0";
        //string addr = "127.0.0.1";
        const char* caddr = addr.c_str();

        if (!::bind(sock,(unsigned char*)caddr,port,err)) {
            cout << "Client Bind Error: " << err << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
        char buf[512];
        string recvfrom_addr = "0.0.0.0";
        //string recvfrom_addr = "127.0.0.1";
        caddr = recvfrom_addr.c_str();
        int bytes = receive_dgram_sync(sock,(unsigned char*)caddr,port,(unsigned char*)buf,512,err);
        if (bytes == -1) {
            cout << sock << " Error receiving: " << err << endl;
        } else {
            cout << sock << " received bytes: " << bytes << endl;
            cout << sock << " msg:" << buf << endl;
        }
    };

    auto th1 = new thread(process);
    this_thread::sleep_for(chrono::milliseconds(10));
    auto th2 = new thread(process);

    this_thread::sleep_for(chrono::milliseconds(300));

    int err;
    SOCKETLIB_HANDLE sock = open_socket_dgam_for_send(err);
    if (sock == 0) {
        cout << "Error: " << err << endl;
    }
    const char* caddr = addr.c_str();

    this_thread::sleep_for(chrono::milliseconds(1000));

    string msg = "THISISASMALLMESSAGE";
    const char* cmsg = msg.c_str();
    //string broadcast_addr = "255.255.255.255";
    //string broadcast_addr = "127.255.255.255";
    string broadcast_addr = "10.0.2.255";
    int bytes = send_dgram(sock,(unsigned char*)broadcast_addr.c_str(),port,(unsigned char*)cmsg,err);
    //int bytes = send_dgram(sock,(unsigned char*)caddr,port,(unsigned char*)cmsg,err);
    if (bytes == -1) {
        cout << "Error: " << err << endl;
    } else {
        cout << "sent bytes " << bytes << " of " << msg.size()+1 << endl;
    }

    th1->join();
    th2->join();
    delete th1;
    delete th2;

    return 0;
}