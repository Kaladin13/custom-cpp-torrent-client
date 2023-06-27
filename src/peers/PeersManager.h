//
// Created by kaladin on 27.06.23.
//

#ifndef TORRENT_PROTOCOL_PEERSMANAGER_H
#define TORRENT_PROTOCOL_PEERSMANAGER_H


#include <vector>
#include "Peer.h"

class PeersManager {
    std::vector<Peer> peers;
public:
    PeersManager() = default;

    void add_peer(const Peer&);

    void print_peers();
    void fetch_peers();
};


#endif //TORRENT_PROTOCOL_PEERSMANAGER_H
