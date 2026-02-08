#include "thread_client_session.hpp"
#include "config_manager.hpp"


ThreadClientSession::ThreadClientSession(int client_fd, ConfigManager &conf_mgr) : _client_fd(client_fd), conf_mgr(conf_mgr), _handler(client_fd, conf_mgr ) {};

void ThreadClientSession::start()
{
    _handler.on_connect();
    _thread = std::thread(&ThreadClientSession::_handle_client, this);
}

ThreadClientSession::~ThreadClientSession()
{
    if (_thread.joinable())
    {
        _thread.join();
    }
}

void ThreadClientSession::_handle_client()
{
    char buffer[128];

    while (true)
    {
        ssize_t n = read(_client_fd, buffer, sizeof(buffer) - 1);
        if (n <= 0)
            break;

        buffer[n] = '\0';

        _handler.on_message(std::string_view(buffer, n));
    }

    close(_client_fd);
    _handler.on_disconnect();
}
