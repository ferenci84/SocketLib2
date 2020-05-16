//
// Created by ferenci84 on 5/16/2020.
//

#include "socketlib2.h"
#include "socketlib2_manage_resources.h"
#include <iostream>

using namespace std;

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall open_socket_dgam(int& error){
    SOCKET sock;
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        error = WSAGetLastError();
        return 0;
    }
    cout << "socket opened: " << sock << endl;
    add_socket(sock);
    return sock;
}

SOCKETLIB2_API int __stdcall send_dgram(SOCKETLIB_HANDLE handle, unsigned char* addr, int port, unsigned char* msg, int& error) {
    SOCKET sock = handle;
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    cout << "send to address: " << ((char*)addr) << " port " << port << endl;
    sockAddr.sin_addr.s_addr = inet_addr((char*)addr);
    int size = strlen((char*)msg)+1;
    cout << "start sending" << endl;
    int ret = sendto(sock,(char*)msg,size,0,(struct sockaddr*) & sockAddr,sizeof(sockAddr));
    cout << "finish sending" << endl;
    if (ret == SOCKET_ERROR) {
        error = WSAGetLastError();
        return -1;
    }
    return ret;
}

SOCKETLIB2_API int __stdcall receive_dgram_sync(SOCKETLIB_HANDLE handle, unsigned char* addr, int port, unsigned char* buffer, int bufsize, int& error) {
    SOCKET sock = handle;
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    cout << "receive from address: " << ((char*)addr) << " port " << port << endl;
    sockAddr.sin_addr.s_addr = inet_addr((char*)addr);
    int fromlen = sizeof(sockAddr);
    cout << "start receiving" << endl;
    int ret = recvfrom(sock,(char*)buffer,bufsize,0,(struct sockaddr*) & sockAddr,&fromlen);
    cout << "finish receiving" << endl;
    if (ret == SOCKET_ERROR) {
        error = WSAGetLastError();
    }
    return ret;
}