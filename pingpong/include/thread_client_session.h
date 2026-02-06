#pragma once

#include "ping_pong_handler.h"
#include <thread>

class ThreadClientSession
{
public:
    explicit ThreadClientSession(int client_fd);
    void start();

    ~ThreadClientSession();

private:
    int _client_fd;
    PingPongHandler _handler;
    std::thread _thread;

    void _handle_client();
};