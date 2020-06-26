//
// Created by ferenci84 on 5/14/2020.
//

#ifndef SOCKETLIB2_SOCKETLIB2_SEND_H
#define SOCKETLIB2_SOCKETLIB2_SEND_H

#include "socketlib2.h"
#include <future>
#include <iostream>
#include <list>
#include <memory>
#include <thread>

using namespace std;



//INTERNAL
bool send_msg_sync(SOCKET sock, const char* msg, int len, int& error) {
    const char* buf = msg;
    int size = len;
    char* bufptr = (char*)buf;
    int remainsize = (int)size;
    while (remainsize > 0) {
        int ret = send(sock, bufptr, remainsize, 0);
        cout << "sent bytes: " << ret << endl;
        if (ret == SOCKET_ERROR) {
            error = WSAGetLastError();
            return false;
        }
        remainsize -= ret;
        bufptr += ret;
    }
    return true;
}

class send_queue {
private:
    SOCKET sock;
    list<string> msgs;
    shared_ptr<thread> th;
    unsigned int max_size;
    int lasterror;
    char* buffer;
    int buffer_pos;
    int buffer_size;
public:
    send_queue(SOCKET sock, unsigned int max_queue_size, unsigned int buffer_size): sock(sock), max_size(max_queue_size), lasterror(0), buffer_size(buffer_size), buffer_pos(0) {
        buffer = new char[buffer_size];
    }
    ~send_queue() {
        if (th->joinable()) th->join();
        delete[] buffer;
    }
private:
    bool send_char(char ch) {
        if (buffer_pos < 0 || buffer_pos >= buffer_size) {
            return false;
        }
        buffer[buffer_pos] = ch;
        buffer_pos++;
        if (buffer_pos == buffer_size) {
            return flush();
        }
        return true;
    }
    bool flush() {
        int err;
        bool succ = ::send_msg_sync(sock,buffer,buffer_pos,err);
        if (!succ){
            cout << "Message sending error: " << err;
            lasterror = err;
        } else {
            cout << "sent msg: " << buffer << endl;
        }
        buffer_pos = 0;
        return succ;
    }

public:
    bool send_msg_sync(const char* msg, unsigned int msg_size) {
        for(const char* it = msg; it != msg+msg_size; ++it){
            if (!send_char(*it)) {
                return false;
            }
        }
        return flush();
    }
    void send_msg_proc(shared_ptr<thread> old_thread) {
        if (old_thread != nullptr && old_thread->joinable()) old_thread->join();
        while(true) {
            if (msgs.empty()) break;
            string msg = msgs.front();
            int msg_size = msg.size()+1;
            msgs.pop_front();
            if (!this->send_msg_sync(msg.c_str(),msg_size)) {
                cout << "Message sending error: " << lasterror << endl;
            }
        }
    }
    bool send_msg_sync(const string& msg) {
        auto msg_cpy = msg;
        int msg_size = msg.size()+1;
        return send_msg_sync(msg_cpy.c_str(),msg_size);
    }
    bool send_msg(const string& msg) {
        if (msgs.size() >= max_size) return false;
        msgs.push_back(msg);
        th = std::make_shared<thread>(&send_queue::send_msg_proc,this,th);
        return true;
    }
    void wait() {
        if (th->joinable()) th->join();
    }
    int get_last_error() {
        return this->lasterror;
    }
};

#endif //SOCKETLIB2_SOCKETLIB2_SEND_H
