#include "socketlib2.h"
#include "socketlib2_manage_resources.h"

#include <iostream>

using namespace std;

bool wsa_startup(int& error) {
    cout << "WSA startup" << endl;
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        error = WSAGetLastError();
        return false;
    }
    return true;
}

bool wsa_cleanup(int& error) {
    cout << "WSA cleanup" << endl;
    if (WSACleanup() != 0) {
        error = WSAGetLastError();
        return false;
    }
    return true;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    int err;
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            cout << "attach" << endl;
            wsa_startup(err);
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            cout << "detach" << endl;
            clean_up();
            wsa_cleanup(err);
            break;
    }
    return TRUE;
}

SOCKETLIB2_API void __stdcall delete_all_socket(int ignored) {
    clean_up();
}


bool stop_send_and_receive(SOCKETLIB_HANDLE sock, int& error) {
    if (shutdown(sock, SD_BOTH) == SOCKET_ERROR) {
        error = WSAGetLastError();
        return false;
    }
    return true;
}

bool close_socket(SOCKETLIB_HANDLE sock, int& error) {
    if (closesocket(sock) == SOCKET_ERROR) {
        error = WSAGetLastError();
        return false;
    }
    return true;
}

SOCKETLIB2_API void __stdcall disconnect_socket(SOCKETLIB_HANDLE sock) {
    if (!socket_exists(sock)) return;
    int err;
    stop_send_and_receive(sock,err);
    close_socket(sock,err);
    cout << "socket closed: " << sock << endl;
    delete_owned_receive_buffer(sock);
    delete_owned_send_queue(sock);
    delete_owned_accept_process(sock);
    remove_socket(sock);
}

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall open_socket(int& error) {
    SOCKET sock;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        error = WSAGetLastError();
        return 0;
    }
    cout << "socket opened: " << sock << endl;
    add_socket(sock);
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

SOCKETLIB2_API bool __stdcall bind_any(SOCKETLIB_HANDLE handle, int port, int& error) {
    SOCKET sock = handle;
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    //TODO: remove logs
    cout << "bind address: " << " port " << port << endl;
    sockAddr.sin_addr.s_addr = INADDR_ANY;
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


SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_server(unsigned char* addr, int port, int queue_size, int& err) {
    SOCKET sock = open_socket(err);
    if (sock == 0) {
        cout << "Failed server socket() call, Error: " << err << endl;
        return 0;
    }
    if (!bind(sock,(unsigned char*)addr,port,err)) {
        cout << "Failed to bind socket, Error: " << err << endl;
        disconnect_socket(sock);
        return 0;
    }
    if (!listen(sock, queue_size,err)) {
        cout << "Failed to listen socket, Error: " << err << endl;
        disconnect_socket(sock);
        return 0;
    }
    cout << "Opened socket: " << sock << endl;
    return sock;
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

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_client(unsigned char* addr, int port, int& error) {
    SOCKET sock = open_socket(error);
    if (sock == 0) {
        cout << "Failed server socket() call, Error: " << error << endl;
        return 0;
    }
    if (!connect(sock, addr,port,error)) {
        cout << "Failed to listen socket, Error: " << error << endl;
        disconnect_socket(sock);
        return 0;
    }
    cout << "connected to server" << endl;
    return sock;
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

