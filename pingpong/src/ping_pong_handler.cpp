#include "ping_pong_handler.h"

#include <print>
#include <unistd.h>

MessageHandler::MessageHandler(int client_fd) : client_fd(client_fd) {}

void MessageHandler::on_connect()
{
    std::println("Client connect, fd = {}", client_fd);
}

void MessageHandler::on_disconnect()
{
    std::println("Client disconnected, fd = {}", client_fd);
}

PingPongHandler::PingPongHandler(int client_fd) : MessageHandler(client_fd) {};

void PingPongHandler::on_message(std::string_view msg)
{
    if (msg.starts_with("ping"))
        write(client_fd, "pong\n", 5);
    else
        write(client_fd, "unknown\n", 8);
}