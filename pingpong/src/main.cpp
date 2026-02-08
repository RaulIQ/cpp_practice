#include <print>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <ping_pong_handler.hpp>
#include <thread_client_session.hpp>
#include <fstream>
#include "config_manager.hpp"

#include <nlohmann/json.hpp>


constexpr const char *SOCKET_PATH = "/tmp/pingpong.sock";


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

    std::vector<std::unique_ptr<ThreadClientSession>> sessions;

    auto cong_mgr = ConfigManager();

    while (true)
    {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0)
        {
            perror("accept");
            continue;
        }

        auto session = std::make_unique<ThreadClientSession>(client_fd, cong_mgr);
        session->start();
        sessions.push_back(std::move(session));
    }

    return 0;
}