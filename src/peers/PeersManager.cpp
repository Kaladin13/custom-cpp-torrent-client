//
// Created by kaladin on 27.06.23.
//

#include <iostream>
#include "PeersManager.h"

void PeersManager::add_peer(const Peer& peer) {
    this->peers.push_back(peer);
}

void PeersManager::print_peers() {
    std::cout << "Got total of " << peers.size() << " peers\n";

    for (auto &p: this->peers) {
        std::cout << "Peer " << p.getPeerId() << ", at " << p.getIp() << ":" << p.getPort() << std::endl;
    }
}

void PeersManager::fetch_peers() {
    for (int i = 0; i < peers.size(); ++i) {
        peers[i].performHandshake();
    }
}
