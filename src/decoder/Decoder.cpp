//
// Created by kaladin on 20.06.23.
//

#include <fstream>
#include "Decoder.h"


Decoder::Decoder(const fs::path &filepath) : filepath(filepath) {
    try {
        // Open the stream to 'lock' the file.
        std::ifstream file(filepath.string(), std::ios::in | std::ios::binary);

        // Obtain the size of the file.
        const auto sz = fs::file_size(filepath);

        // Create a buffer.
        std::string result(sz, '\0');

        // Read the whole file into the buffer.
        file.read(result.data(), sz);

        this->decoded_content = bencode::decode(result);
    }
    catch (std::exception &e) {
        std::cerr << "Error when initializing decoder: " << e.what() << std::endl;
    }
}

void Decoder::decode_as_json() {
    auto dict = std::get<bencode::dict>(decoded_content);

    decode_dict(dict, 0);
}

void Decoder::decode_dict(const bencode::dict &dict, int level) {
    for (const auto &v: dict) {
        std::cout << std::string(level, ' ') << v.first << ": \n";

        decode_basic_data(v.second, level);
    }
}

void Decoder::decode_basic_data(const bencode::data &data, int level) {
    if (auto value = std::get_if<bencode::string>(&data)) {
        std::cout << std::string(level + 1, ' ') << *value << std::endl;
        return;
    }

    if (auto value = std::get_if<bencode::integer>(&data)) {
        std::cout << std::string(level + 1, ' ') << *value << std::endl;
        return;
    }

    if (auto value = std::get_if<bencode::dict>(&data)) {
        std::cout << std::string(level + 1, ' ') << "{\n";
        decode_dict(*value, level + 2);
        std::cout << std::string(level + 1, ' ') << "}\n";

        return;
    }

    if (auto value = std::get_if<bencode::list>(&data)) {
        std::cout << std::string(level + 1, ' ') << "[\n";

        auto bb = *value;

        for (const auto &elem: bb) {
            decode_basic_data(elem, level + 1);
        }

        std::cout << std::string(level + 1, ' ') << "]\n";
    }
}

torrent Decoder::decode_as_torrent() {
    torrent meta;

    auto dict = std::get<bencode::dict>(decoded_content);

    auto announce = dict.find("announce");

    if (announce != dict->end()) {
        meta.setAnnounceUrl(std::get<bencode::string>(announce->second));
    }

    auto info = dict.find("info");

    if (info == dict->end()) {
        throw "No info in torrent file";
    }

    auto info_dict = std::get<bencode::dict>(info->second);

    auto bencoded_info_dict = bencode::encode(info_dict);
    meta.setEncodedInfo(bencoded_info_dict);
    torrent::pub_encoded_info = bencoded_info_dict;

    auto name = std::get<bencode::string>(info_dict.find("name")->second);
    auto length = std::get<bencode::integer>(info_dict.find("length")->second);
    auto pieces = std::get<bencode::string>(info_dict.find("pieces")->second);
    auto is_private = std::get_if<bencode::integer>(&info_dict.find("private")->second);
    auto piece_length = std::get<bencode::integer>(info_dict.find("piece length")->second);

    meta.setFilename(name);
    meta.setIsPrivate(is_private != nullptr && (bool) *is_private);
    meta.setLength(length);
    meta.setPieceLength(piece_length);
    meta.setPieces(pieces);

    return meta;
}

std::vector<std::pair<std::string, uint16_t>> parsePeers(const std::string &peersBinary) {
    std::vector<std::pair<std::string, uint16_t>> result;

    for (size_t i = 0; i < peersBinary.length(); i += 6) {
        // Extract the IP address (first 4 bytes)
        std::string ipAddress;
        for (size_t j = i; j < i + 4; j++) {
            auto byte = static_cast<unsigned char>(peersBinary[j]);
            ipAddress += std::to_string(byte);
            if (j != i + 3) {
                ipAddress += ".";
            }
        }

        // Extract the port number (last 2 bytes)
        uint16_t port = (static_cast<unsigned char>(peersBinary[i + 4]) << 8) |
                        static_cast<unsigned char>(peersBinary[i + 5]);

        result.emplace_back(ipAddress, port);
    }

    return result;
}

PeersManager Decoder::decode_tracker_response(const std::string &value) {
    auto response_raw = bencode::decode(value);

    auto response_dict = std::get<bencode::dict>(response_raw);

    auto seeders = std::get<bencode::integer>(response_dict.find("complete")->second);
    std::cout << "There is total of " << seeders << " seeders!\n";

    PeersManager peersManager;

    auto peers_list = std::get_if<bencode::list>(&response_dict.find("peers")->second);

    if (peers_list) {
        for (auto &i: *peers_list) {
            auto dd = std::get<bencode::dict>(i);

            auto port = std::get<bencode::integer>(dd.find("port")->second);
            auto ip = std::get<bencode::string>(dd.find("ip")->second);
            auto peer_id = std::get<bencode::string>(dd.find("peer id")->second);

            peersManager.add_peer({peer_id, ip, static_cast<int>(port)});
        }

        return peersManager;
    }

    auto peers_string = std::get<bencode::string>(response_dict.find("peers")->second);

    auto peers_parsed = parsePeers(peers_string);

    for (auto &p: peers_parsed) {
        peersManager.add_peer({"undefined_id", p.first, p.second});
    }

    return peersManager;
}


std::string Decoder::decodeHex(const std::string &value) {
    int hashLength = value.length();
    std::string decodedHexString;

    for (int i = 0; i < hashLength; i += 2) {
        std::string byte = value.substr(i, 2);
        char c = (char) (int) strtol(byte.c_str(), nullptr, 16);
        decodedHexString.push_back(c);
    }

    return decodedHexString;
}
