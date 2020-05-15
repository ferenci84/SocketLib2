#ifndef SOCKETLIB2_SOCKETLIB2_H
#define SOCKETLIB2_SOCKETLIB2_H

#ifdef SocketLib2_EXPORTS
#define SOCKETLIB2_API __declspec(dllexport)
#else
#define SOCKETLIB2_API __declspec(dllimport)
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "WinSock2.h"
#include "Ws2tcpip.h"
#include <cstdint>
#include <string>

using namespace std;

#ifdef _WIN64
typedef uint64_t SOCKETLIB_HANDLE;
typedef uint64_t SOCKETLIB_PTR;
#else
typedef uint32_t SOCKETLIB_HANDLE;
typedef uint32_t SOCKETLIB_PTR;
#endif

SOCKETLIB2_API void __stdcall delete_all_socket(int ignored);

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall open_socket(int& error);
SOCKETLIB2_API void __stdcall disconnect_socket(SOCKETLIB_HANDLE sock);
//TODO: store allocated resources, and remove them when disconnecting socket

SOCKETLIB2_API bool __stdcall bind(SOCKETLIB_HANDLE handle, unsigned char* addr, int port, int& error);
SOCKETLIB2_API bool __stdcall listen(SOCKETLIB_HANDLE handle, int queue_size, int& error);
SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_server(unsigned char* addr, int port, int queue_size, int& err);

SOCKETLIB2_API bool __stdcall connect(SOCKETLIB_HANDLE handle, unsigned char* addr, int port, int& error);
SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_client(unsigned char* addr, int port, int& error);

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_accept_process(SOCKETLIB_HANDLE socket);
SOCKETLIB2_API void __stdcall delete_accept(SOCKETLIB_HANDLE accept_handle);
SOCKETLIB2_API bool __stdcall poll_accept(SOCKETLIB_HANDLE accept_handle, int wait_for, bool restart);
SOCKETLIB2_API bool __stdcall get_accept_result(SOCKETLIB_HANDLE accept_handle, SOCKETLIB_HANDLE& client_sock, unsigned char* addr_buf, unsigned int addr_buf_size, unsigned short& port, int& error);

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_send_queue(SOCKETLIB_HANDLE sock, unsigned int max_queue_size, unsigned int buffer_size);
SOCKETLIB2_API void __stdcall delete_send_queue(SOCKETLIB_HANDLE queue_handle);
SOCKETLIB2_API int __stdcall get_queue_error(SOCKETLIB_HANDLE queue_handle);
SOCKETLIB2_API void __stdcall send_msg(SOCKETLIB_HANDLE queue_handle, unsigned char* msg);

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_receive_buffer(SOCKETLIB_HANDLE sock, unsigned int size);
SOCKETLIB2_API void __stdcall delete_receive_buffer(SOCKETLIB_HANDLE buffer);
SOCKETLIB2_API bool __stdcall poll_msg(SOCKETLIB_HANDLE buffer, int wait_ms, bool restart);
SOCKETLIB2_API bool __stdcall get_msg(SOCKETLIB_HANDLE buffer, unsigned char* msg_buf, int buf_size, int& err);

#endif //SOCKETLIB2_SOCKETLIB2_H
