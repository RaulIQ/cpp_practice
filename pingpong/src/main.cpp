#include <print>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

constexpr const char *SOCKET_PATH = "/tmp/pingpong.sock";

class MessageHandler
{
public:
    explicit MessageHandler(int client_fd) : client_fd(client_fd) {}

    virtual ~MessageHandler() = default;

    virtual void on_message(std::string_view msg) = 0;

    void on_disconnect()
    {
        std::println("Client disconnected, fd = {}", client_fd);
    }

    void on_connect()
    {
        std::println("Client connect, fd = {}", client_fd);
    }

protected:
    int client_fd;
};

class PingPongHandler : public MessageHandler
{
public:
    explicit PingPongHandler(int client_fd) : MessageHandler(client_fd) {}
    void on_message(std::string_view msg) override
    {
        if (msg.starts_with("ping"))
            write(client_fd, "pong\n", 5);
        else
            write(client_fd, "unknown\n", 8);
    }
};

class ClientSession
{
public:
    explicit ClientSession(int client_fd) : _client_fd(client_fd), _handler(client_fd)
    {
    }

    void start()
    {
        _handler.on_connect();
        _thread = std::thread(&ClientSession::_handle_client, this);
    }

    ~ClientSession()
    {
        if (_thread.joinable())
        {
            _thread.join();
        }
    }

private:
    int _client_fd;
    PingPongHandler _handler;
    std::thread _thread;

    void _handle_client()
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
};

int main()
{
    unlink(SOCKET_PATH);

    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("socket");
        return 1;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
    {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("listen");
        return 1;
    }

    std::println(SOCKET_PATH);

    std::vector<std::unique_ptr<ClientSession>> sessions;

    while (true)
    {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0)
        {
            perror("accept");
            continue;
        }

        auto session = std::make_unique<ClientSession>(client_fd);
        session->start();
        sessions.push_back(std::move(session));
    }

    return 0;
}