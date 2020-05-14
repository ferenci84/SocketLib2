#include "socketlib2.h"

#include <iostream>

using namespace std;

SOCKETLIB2_API bool __stdcall wsa_startup(int& error) {
    cout << "WSA startup" << endl;
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        error = WSAGetLastError();
        return false;
    }
    return true;
}

SOCKETLIB2_API bool __stdcall wsa_cleanup(int& error) {
    cout << "WSA cleanup" << endl;
    if (WSACleanup() != 0) {
        error = WSAGetLastError();
        return false;
    }
    return true;
}

SOCKETLIB2_API bool __stdcall stop_receive(SOCKETLIB_HANDLE sock, int& error) {
    if (shutdown(sock, SD_RECEIVE) == SOCKET_ERROR) {
        error = WSAGetLastError();
        return false;
    }
    return true;
}

SOCKETLIB2_API bool __stdcall stop_send(SOCKETLIB_HANDLE sock, int& error) {
    if (shutdown(sock, SD_SEND) == SOCKET_ERROR) {
        error = WSAGetLastError();
        return false;
    }
    return true;
}

SOCKETLIB2_API bool __stdcall stop_send_and_receive(SOCKETLIB_HANDLE sock, int& error) {
    if (shutdown(sock, SD_BOTH) == SOCKET_ERROR) {
        error = WSAGetLastError();
        return false;
    }
    return true;
}

SOCKETLIB2_API bool __stdcall close_socket(SOCKETLIB_HANDLE sock, int& error) {
    if (closesocket(sock) == SOCKET_ERROR) {
        error = WSAGetLastError();
        return false;
    }
    return true;
}

SOCKETLIB2_API void __stdcall disconnect_socket_and_forget(SOCKETLIB_HANDLE sock) {
    int err;
    stop_send_and_receive(sock,err);
    close_socket(sock,err);
}

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall open_socket(int& error) {
    SOCKET sock;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        error = WSAGetLastError();
        return 0;
    }
    return sock;
}

SOCKETLIB2_API bool __stdcall bind(SOCKETLIB_HANDLE handle, unsigned char* addr, int port, int& error) {
    SOCKET sock = handle;
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    //TODO: remove logs
    cout << "bind address: " << ((char*)addr) << " port " << port << endl;
    sockAddr.sin_addr.s_addr = inet_addr((char*)addr);
    if (::bind(sock, (struct sockaddr*) & sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
        error = WSAGetLastError();
        return false;
    }
    return true;
}

SOCKETLIB2_API bool __stdcall listen(SOCKETLIB_HANDLE handle, int queue_size, int& error) {
    SOCKET sock = handle;
    if (listen(sock, queue_size) != 0) {
        error = WSAGetLastError();
        return false;
    }
    return true;
}

SOCKETLIB2_API bool __stdcall connect(SOCKETLIB_HANDLE handle, unsigned char* addr, int port, int& error) {
    SOCKET sock = handle;

    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    sockAddr.sin_addr.s_addr = inet_addr((char*)addr);

    //TODO: remove logs
    cout << "connecting" << endl;
    if (connect(sock, (struct sockaddr*) & sockAddr, sizeof(sockAddr)) != 0) {
        error = WSAGetLastError();
        return false;
    }
    cout << "connected to server" << endl;

    return true;
}

SOCKETLIB2_API bool __stdcall test_sgntr1(int i1, int i2) { return 0; }
SOCKETLIB2_API bool __stdcall test_sgntr2(int i1, int i2) { return 0; }
SOCKETLIB2_API bool __stdcall test_sgntr3(int i1, int i2, int& i3) { return 0; }
SOCKETLIB2_API bool __stdcall test_sgntr4(int i1, int i2, int& i3, int& i4) { return 0; }
SOCKETLIB2_API bool __stdcall test_sgntr5(int i1, int& i3, int& i4) { return 0; }
SOCKETLIB2_API bool __stdcall test_sgntr6(int& i3, int& i4) { return 0; }
SOCKETLIB2_API bool __stdcall test_sgntr7(int i1, int i2, bool& i3, int& i4) { return 0; }
SOCKETLIB2_API bool __stdcall test_sgntr8(int i1, int i2, int& i3, bool& i4) { return 0; }


// UTILITY FUNCTIONS

