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

#ifdef _WIN64
typedef uint64_t SOCKETLIB_HANDLE;
typedef uint64_t SOCKETLIB_PTR;
#else
typedef uint32_t SOCKETLIB_HANDLE;
typedef uint32_t SOCKETLIB_PTR;
#endif



SOCKETLIB2_API bool __stdcall wsa_startup(int& error);

SOCKETLIB2_API bool __stdcall wsa_cleanup(int& error);

SOCKETLIB2_API bool __stdcall stop_receive(SOCKETLIB_HANDLE sock, int& error);
SOCKETLIB2_API bool __stdcall stop_send(SOCKETLIB_HANDLE sock, int& error);
SOCKETLIB2_API bool __stdcall stop_send_and_receive(SOCKETLIB_HANDLE sock, int& error);
SOCKETLIB2_API bool __stdcall close_socket(SOCKETLIB_HANDLE sock, int& error);
SOCKETLIB2_API void __stdcall disconnect_socket_and_forget(SOCKETLIB_HANDLE sock);

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall open_socket(int& error);
SOCKETLIB2_API bool __stdcall bind(SOCKETLIB_HANDLE handle, unsigned char* addr, int port, int& error);
SOCKETLIB2_API bool __stdcall listen(SOCKETLIB_HANDLE handle, int queue_size, int& error);

SOCKETLIB2_API bool __stdcall connect(SOCKETLIB_HANDLE handle, unsigned char* addr, int port, int& error);

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall accept(SOCKETLIB_HANDLE sock);
SOCKETLIB2_API bool __stdcall poll_accept(SOCKETLIB_HANDLE task_handle, int wait_for_ms, SOCKETLIB_HANDLE& client_socket, unsigned char* client_addr_buf, unsigned int client_addr_buf_size, unsigned short& client_port, int& error);

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall send(SOCKETLIB_HANDLE sock, unsigned char* msg, int len);
SOCKETLIB2_API bool __stdcall poll_send(SOCKETLIB_HANDLE task_handle, int wait_for_ms);
SOCKETLIB2_API bool __stdcall finish_send(SOCKETLIB_HANDLE task_handle, int& error);

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall receive(SOCKETLIB_HANDLE sock, unsigned char* buf, int buffer_size);
SOCKETLIB2_API bool __stdcall poll_receive(SOCKETLIB_HANDLE task_handle, int wait_for_ms);
SOCKETLIB2_API int __stdcall finish_receive(SOCKETLIB_HANDLE task_handle, int& error);

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_send_queue(SOCKETLIB_HANDLE sock, unsigned int max_queue_size, unsigned int buffer_size);
SOCKETLIB2_API void __stdcall delete_send_queue(SOCKETLIB_HANDLE queue_handle);
SOCKETLIB2_API int __stdcall get_queue_error(SOCKETLIB_HANDLE queue_handle);
SOCKETLIB2_API void __stdcall send_msg(SOCKETLIB_HANDLE queue_handle, unsigned char* msg);

SOCKETLIB2_API SOCKETLIB_HANDLE __stdcall create_receive_buffer(SOCKETLIB_HANDLE sock, unsigned int size);
SOCKETLIB2_API void __stdcall delete_receive_buffer(SOCKETLIB_HANDLE buffer);
SOCKETLIB2_API bool __stdcall poll_msg(SOCKETLIB_HANDLE buffer, int wait_ms, unsigned char* msg_buf, int buf_size, int& err);

#endif //SOCKETLIB2_SOCKETLIB2_H
