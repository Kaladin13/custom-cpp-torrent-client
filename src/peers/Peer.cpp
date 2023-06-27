//
// Created by kaladin on 27.06.23.
//

#include "Peer.h"
#include "../net/socket_data.h"
#include "../decoder/Decoder.h"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <utility>
#include <cassert>

Peer::Peer(std::string peerId, std::string ip, int port) : peer_id(std::move(peerId)), ip(std::move(ip)), port(port) {}

const std::string &Peer::getPeerId() const {
    return peer_id;
}

const std::string &Peer::getIp() const {
    return ip;
}

int Peer::getPort() const {
    return port;
}

void Peer::connect() {
    int sock;
    struct sockaddr_in address{};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Socket creation error: " + std::to_string(sock));

    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    char *tempIp = new char[ip.length() + 1];
    strcpy(tempIp, ip.c_str());

    // Converts IP address from string to struct in_addr
    if (inet_pton(AF_INET, tempIp, &address.sin_addr) <= 0)
        throw std::runtime_error("Invalid IP address: " + ip);

    // Sets socket to non-block mode
    if (!setSocketBlocking(sock, false))
        throw std::runtime_error("An error occurred when setting socket " + std::to_string(sock) + "to NONBLOCK");

    ::connect(sock, (struct sockaddr *) &address, sizeof(address));

    fd_set fdset;
    struct timeval tv{};
    FD_ZERO(&fdset);
    FD_SET(sock, &fdset);
    const auto CONNECT_TIMEOUT = 3;
    tv.tv_sec = CONNECT_TIMEOUT;
    tv.tv_usec = 0;

    if (select(sock + 1, nullptr, &fdset, nullptr, &tv) == 1) {
        int so_error;
        socklen_t len = sizeof so_error;

        getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);

        if (so_error == 0) {
            // Sets socket to blocking mode
            if (!setSocketBlocking(sock, true))
                throw std::runtime_error("An error occurred when setting socket " + std::to_string(sock) + "to BLOCK");
            this->sockfd = sock;
            return;
        }
    }
    close(sock);
    throw std::runtime_error("Connect to " + ip + ": FAILED [Connection timeout]");
}

void Peer::performHandshake() {
    connect();

    const std::string protocol = "BitTorrent protocol";

    std::stringstream buffer;

    buffer << (char) protocol.length();
    buffer << protocol;

    std::string reserved;
    for (int i = 0; i < 8; i++)
        reserved.push_back('\0');

    buffer << reserved;
    buffer << Decoder::decodeHex(torrent::pub_encoded_info);
    buffer << "clientId";

    assert (buffer.str().length() == protocol.length() + 49);

    sendData(this->sockfd, buffer.str());

    std::cout << "Handshake sent successfully" << std::endl;
}

bool Peer::setSocketBlocking(int sock, bool blocking) {
    if (sock < 0)
        return false;

    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) return false;
    flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
    return (fcntl(sock, F_SETFL, flags) == 0);
}
