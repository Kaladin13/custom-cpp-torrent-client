//
// Created by kaladin on 27.06.23.
//

#ifndef TORRENT_PROTOCOL_SOCKET_DATA_H
#define TORRENT_PROTOCOL_SOCKET_DATA_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <chrono>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <limits>

void sendData(const int sock, const std::string &data) {
    int n = data.length();
    char buffer[n];
    for (int i = 0; i < n; i++)
        buffer[i] = data[i];

    int res = send(sock, buffer, n, 0);
    if (res < 0)
        throw std::runtime_error("Failed to write data to socket " + std::to_string(sock));
}

#endif //TORRENT_PROTOCOL_SOCKET_DATA_H
