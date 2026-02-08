#pragma once

#include "ping_pong_handler.hpp"
#include <thread>

class ThreadClientSession
{
public:
    explicit ThreadClientSession(int client_fd, ConfigManager &conf_mgr);

    ConfigManager &conf_mgr;

    ThreadClientSession(int client_fd);

    void start();

    ~ThreadClientSession();

private:
    int _client_fd;
    PingPongHandler _handler;
    std::thread _thread;

    void _handle_client();
};