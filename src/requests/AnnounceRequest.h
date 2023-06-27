//
// Created by kaladin on 27.06.23.
//

#ifndef TORRENT_PROTOCOL_ANNOUNCEREQUEST_H
#define TORRENT_PROTOCOL_ANNOUNCEREQUEST_H


#include <string>
#include <vector>
#include "../torrent/torrent.h"
#include "../peers/PeersManager.h"

#define TRACKER_TIMEOUT 13000

class AnnounceRequest {
    std::string info_hash;
    std::string peer_id;
    long long left{};
    std::string base;

    static std::string sha1_hash(const std::string &);

    static std::vector<std::string> baseURLGetter(const std::string &);

public:
    AnnounceRequest() = default;

    explicit AnnounceRequest(const torrent &torrent);

    PeersManager make_tracker_response();
};


#endif //TORRENT_PROTOCOL_ANNOUNCEREQUEST_H
