#include "ping_pong_handler.hpp"

#include <print>
#include <unistd.h>
#include <chrono>
#include <format>

MessageHandler::MessageHandler(int client_fd, ConfigManager &conf_mgr) : client_fd(client_fd), conf_mgr(conf_mgr) {}

void MessageHandler::on_connect()
{
    std::println("Client connect, fd = {}", client_fd);

    MessageRecord record{
        .fd = client_fd,
        .type = MessageType::Connect,
        .timestamp = std::chrono::system_clock::now()
    };

    conf_mgr.append(record.to_json());
}

void MessageHandler::on_disconnect()
{
    std::println("Client disconnected, fd = {}", client_fd);

    MessageRecord record{
        .fd = client_fd,
        .type = MessageType::Disconnect,
        .timestamp = std::chrono::system_clock::now()
    };

    conf_mgr.append(record.to_json());
}

PingPongHandler::PingPongHandler(int client_fd, ConfigManager &conf_mgr) : MessageHandler(client_fd, conf_mgr) {};

void PingPongHandler::on_message(std::string_view msg)
{
    if (msg.starts_with("ping")) {
        MessageRecord record{
            .fd = client_fd,
            .type = MessageType::Ping,
            .timestamp = std::chrono::system_clock::now()
        };
    
        conf_mgr.append(record.to_json());

        write(client_fd, "pong\n", 5);
    }
    
    else {
        MessageRecord record{
            .fd = client_fd,
            .type = MessageType::Unknown,
            .timestamp = std::chrono::system_clock::now()
        };
    
        conf_mgr.append(record.to_json());

        write(client_fd, "unknown\n", 8);
    }
}

using json = nlohmann::json;

static std::string to_string(MessageType type)
{
    switch (type)
    {
        case MessageType::Ping: return "ping";
        case MessageType::Connect: return "connect";
        case MessageType::Disconnect: return "disconnect";
        default: return "unknown";
    }
}

std::string format_timestamp(std::chrono::system_clock::time_point tp)
{
    return std::format(
        "{:%Y-%m-%dT%H:%M:%SZ}",
        std::chrono::floor<std::chrono::seconds>(tp)
    );
}

json MessageRecord::to_json() const
{
    auto time = std::chrono::system_clock::now();

    return {
        { "fd", fd },
        { "type", to_string(type) },
        { "timestamp", format_timestamp(time) }
    };
}



