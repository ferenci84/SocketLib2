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

//INTERNAL
bool receive_msg_sync(SOCKET sock, char* buf, int buffer_size, int& bytes, bool& disconnected, int& error) {
    int ret = recv(sock, buf, buffer_size, 0);
    if (ret == SOCKET_ERROR) {
        error = WSAGetLastError();
        bytes = -1;
        return false;
    }
    if (ret == 0) {
        disconnected = true;
        bytes = 0;
        return false;
    }
    disconnected = false;
    bytes = ret;
    return true;
}

//INTERNAL
struct receive_task_result {
    bool success;
    bool disconnected;
    int bytes_received;
    int error;
};

//INTERNAL
class receive_task {
private:
    future<receive_task_result> task;
public:
    explicit receive_task(SOCKETLIB_HANDLE sock, char* buf, int buffer_size) {
        task = async(launch::async, [&](SOCKETLIB_HANDLE sock, char* buf, int buffer_size) {
            receive_task_result res{};
            res.success = receive_msg_sync(sock,buf,buffer_size,res.bytes_received,res.disconnected,res.error);
            return res;
        },sock, buf, buffer_size);
    }
    future<receive_task_result>* get_task() {
        return &task;
    }
};

class msg_buffer {
    char* buffer;
    int buffer_size;
    //int buffer_current_size;
    char* buffer_end;
    char* buffer_pos;
    SOCKET sock;
    bool connected;
    future<string> msg_future;
    bool future_in_progress;
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
    bool get_char(char* ch) {
        if (buffer_pos == buffer_end) {
            int ret = recv(sock, buffer, buffer_size, 0);
            if (ret == 0) {
                connected = false;
                return false;
            }
            if (ret == SOCKET_ERROR) {
                cout << "socket error: " << WSAGetLastError() << endl;
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
    string get_msg()  {
        stringstream ss;
        char ch;
        bool success;
        while ((success = get_char(&ch)) && ch != 0) {
            cout << "received char: " << ch << endl;
            ss << ch;
        }
        if (success) {
            return ss.str();
        }
        else {
            return string();
        }
    }
    bool poll_msg(string& str, int wait_for, bool restart = true) {
        if (!this->is_connected()) return false;
        bool start_task = false;
        if (!future_in_progress) {
            cout << "starting recv task" << endl;
            msg_future = async(launch::async, [&] {
                string str = this->get_msg();
                cout << "received string: " << str;
                return str;
            });
            future_in_progress = true;
        }
        if (msg_future.wait_for(std::chrono::milliseconds(wait_for)) == future_status::ready) {
            str = msg_future.get();
            if (restart) {
                cout << "restarting recv task" << endl;
                msg_future = async(launch::async, [&] {
                    string str = this->get_msg();
                    cout << "received string: " << str;
                    return str;
                });
            } else {
                future_in_progress = false;
            }
            return true;
        }
        return false;
    }

};

#endif //SOCKETLIB2_SOCKETLIB2_RECEIVE_H
