//
// Created by ferenci84 on 5/14/2020.
//

#ifndef SOCKETLIB2_SOCKETLIB2_RECEIVE_H
#define SOCKETLIB2_SOCKETLIB2_RECEIVE_H

#include "socketlib2.h"
#include <future>
#include <iostream>
#include <sstream>

using namespace std;

struct msg_result {
    string msg;
    bool success{};
    int error{};
};

class msg_buffer {
    char* buffer;
    int buffer_size;
    //int buffer_current_size;
    char* buffer_end;
    char* buffer_pos;
    SOCKET sock;
    bool connected;
    future<msg_result> msg_future;
    bool future_in_progress;
    msg_result res;
public:
    msg_buffer(SOCKET sock, unsigned int size) : future_in_progress(false), sock(sock), buffer_size(size), connected(true) {
        buffer = new char[size];
        buffer_pos = buffer;
        buffer_end = buffer;
    }
    ~msg_buffer() {
        if (future_in_progress) {
            msg_future.get();
        }
        delete[] buffer;
    }
private:
    //TODO: add error
    bool get_char(char* ch, int& error) {
        if (buffer_pos == buffer_end) {
            int ret = recv(sock, buffer, buffer_size, 0);
            if (ret == 0) {
                connected = false;
                return false;
            }
            if (ret == SOCKET_ERROR) {
                error = WSAGetLastError();
                //TODO: add specific errors for disconnection
                cout << "socket error: " << error << endl;
                connected = false;
                return false;
            }
            buffer_end = buffer + ret;
            buffer_pos = buffer;
        }
        if (buffer_pos >= buffer && buffer_pos < buffer_end) {
            *ch = *buffer_pos;
            buffer_pos++;
            return true;
        } else {
            cout << "invalid buffer position" << endl;
            return false;
        }

    }
public:
    bool is_connected() {
        return connected;
    }
    msg_result get_msg()  {
        stringstream ss;
        char ch;
        msg_result res{};
        while ((res.success = get_char(&ch,res.error)) && ch != 0) {
            cout << "received char: " << ch << endl;
            ss << ch;
        }
        if (res.success) {
            res.msg = ss.str();
        }
        return res;
    }
    bool poll_msg(int wait_for, bool restart = true) {
        //if (!this->is_connected()) return false;
        bool start_task = false;
        if (!future_in_progress) {
            cout << "starting recv task" << endl;
            msg_future = async(launch::async, &msg_buffer::get_msg, this);
            future_in_progress = true;
        }
        if (msg_future.wait_for(std::chrono::milliseconds(wait_for)) == future_status::ready) {
            res = msg_future.get();
            if (restart) {
                cout << "restarting recv task" << endl;
                msg_future = async(launch::async, &msg_buffer::get_msg, this);
            } else {
                future_in_progress = false;
            }
            return true;
        }
        return false;
    }
    bool get_last_result(string& str, int& error) {
        if (res.success) {
            str = res.msg;
        } else {
            error = res.error;
        }
        return res.success;
    }

};

#endif //SOCKETLIB2_SOCKETLIB2_RECEIVE_H
