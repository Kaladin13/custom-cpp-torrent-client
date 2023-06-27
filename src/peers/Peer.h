//
// Created by kaladin on 27.06.23.
//

#ifndef TORRENT_PROTOCOL_PEER_H
#define TORRENT_PROTOCOL_PEER_H


#include <string>

class Peer {
    std::string peer_id;
    std::string ip;
    int port;

    int sockfd;

    void connect();
public:
    Peer(std::string peerId, std::string ip, int port);

    const std::string &getPeerId() const;

    const std::string &getIp() const;

    int getPort() const;

    void performHandshake();

    bool setSocketBlocking(int sock, bool b);
};


#endif //TORRENT_PROTOCOL_PEER_H
