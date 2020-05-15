//
// Created by ferenci84 on 5/15/2020.
//

#ifndef SOCKETLIB2_SOCKETLIB2_MANAGE_RESOURCES_H
#define SOCKETLIB2_SOCKETLIB2_MANAGE_RESOURCES_H

#include "socketlib2.h"

/* cleanup tasks:
 *  * when cleaning up: remove all sockets
 *  * when removing socket:
 *      * remove from set
 *      * remove mapped accept_process if exists
 *      * remove mapped send_queue if exists
 *      * remove mapped receive_buffer if exists
 *  * when creating resource
 *      * check if already exists
 *      * if so, retrieve existing
 *  * when removing resource:
 *      * check if resource exists
 *      * delete from set
 * */

void clean_up();
void add_socket(SOCKETLIB_HANDLE socket);
void remove_socket(SOCKETLIB_HANDLE socket);
bool socket_exists(SOCKETLIB_HANDLE socket);
void add_accept_process(SOCKETLIB_HANDLE socket, SOCKETLIB_HANDLE accept_process);
SOCKETLIB_HANDLE retrieve_accept_process(SOCKETLIB_HANDLE socket);
void remove_accept_process(SOCKETLIB_HANDLE handle);
bool accept_process_exists(SOCKETLIB_HANDLE handle);
void delete_owned_accept_process(SOCKETLIB_HANDLE socket);
void add_send_queue(SOCKETLIB_HANDLE socket, SOCKETLIB_HANDLE send_queue);
SOCKETLIB_HANDLE retrieve_send_queue(SOCKETLIB_HANDLE socket);
void remove_send_queue(SOCKETLIB_HANDLE handle);
bool send_queue_exists(SOCKETLIB_HANDLE handle);
void delete_owned_send_queue(SOCKETLIB_HANDLE socket);;
void add_receive_buffer(SOCKETLIB_HANDLE socket, SOCKETLIB_HANDLE receive_buffer);
SOCKETLIB_HANDLE retrieve_receive_buffer(SOCKETLIB_HANDLE socket);
void remove_receive_buffer(SOCKETLIB_HANDLE handle);
bool receive_buffer_exists(SOCKETLIB_HANDLE handle);
void delete_owned_receive_buffer(SOCKETLIB_HANDLE socket);

#endif //SOCKETLIB2_SOCKETLIB2_MANAGE_RESOURCES_H
