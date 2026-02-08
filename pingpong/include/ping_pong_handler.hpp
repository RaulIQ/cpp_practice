#pragma once

#include <string_view>
#include "config_manager.hpp"
#include <nlohmann/json.hpp>

class MessageHandler
{
public:
    explicit MessageHandler(int client_fd, ConfigManager &conf_mgr);

    ConfigManager &conf_mgr;

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
    explicit PingPongHandler(int client_fd, ConfigManager &conf_mgr);
    void on_message(std::string_view msg) override;
};

enum class MessageType
{
    Connect,
    Ping,
    Disconnect,
    Unknown
};

struct MessageRecord
{
    int fd;
    MessageType type;
    std::chrono::system_clock::time_point timestamp;

    nlohmann::json to_json() const;
};