//
// Created by ferenci84 on 5/14/2020.
//

#ifndef SOCKETLIB2_SOCKETLIB2_ACCEPT_H
#define SOCKETLIB2_SOCKETLIB2_ACCEPT_H

#include "socketlib2.h"

SOCKETLIB_HANDLE accept_sync(SOCKET sock, struct sockaddr_in& connectSAddr, int& error);

#endif //SOCKETLIB2_SOCKETLIB2_ACCEPT_H
