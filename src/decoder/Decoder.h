//
// Created by kaladin on 20.06.23.
//

#ifndef TORRENT_PROTOCOL_DECODER_H
#define TORRENT_PROTOCOL_DECODER_H


#include <string>
#include <filesystem>
#include "bencode/bencode.hpp"
#include "../torrent/torrent.h"
#include "../peers/PeersManager.h"

namespace fs = std::filesystem;

class Decoder {
    const fs::path &filepath;
    bencode::data decoded_content;

    void decode_dict(const bencode::dict& dict, int level);
    void decode_basic_data(const bencode::data& data, int level);

public:
    explicit Decoder(const fs::path &filepath);
    static std::string decodeHex(const std::string &value);

    void decode_as_json();
    torrent decode_as_torrent();
    static PeersManager decode_tracker_response(const std::string& value);
};


#endif //TORRENT_PROTOCOL_DECODER_H
