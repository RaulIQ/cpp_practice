#pragma once

#include <string_view>

class MessageHandler
{
public:
    explicit MessageHandler(int client_fd);
    virtual ~MessageHandler() = default;

    virtual void on_message(std::string_view msg) = 0;
    void on_connect();
    void on_disconnect();

protected:
    int client_fd;
};

class PingPongHandler : public MessageHandler
{
public:
    explicit PingPongHandler(int client_fd);
    void on_message(std::string_view msg) override;
};