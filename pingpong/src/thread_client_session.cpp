#include "thread_client_session.h"

ThreadClientSession::ThreadClientSession(int client_fd) : _client_fd(client_fd), _handler(client_fd) {};

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
