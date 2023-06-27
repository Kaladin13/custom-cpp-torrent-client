//
// Created by kaladin on 27.06.23.
//

#include "AnnounceRequest.h"
#include "sha1/sha1.h"
#include "../decoder/Decoder.h"
#include "../decoder/utility.h"
#include <cpr/cpr.h>

PeersManager AnnounceRequest::make_tracker_response() {
    auto splits = baseURLGetter(this->base);

    auto params = cpr::Parameters{{"info_hash",  std::string(binaryStringDecoding(this->info_hash))},
                                  {"peer_id",    this->peer_id},
                                  {"left",       std::to_string(this->left)},
                                  {"port",       std::to_string(15647)},
                                  {"uploaded",   std::to_string(0)},
                                  {"downloaded", std::to_string(0)}};

    if (splits.size() != 1) {
        this->base = splits[0];
        params.Add({splits[1], splits[2]});
    }

//    cpr::Response r = cpr::Get(cpr::Url{this->base}, params, cpr::Timeout{TRACKER_TIMEOUT});
//
//    if (r.status_code == 200 || !r.error) {
//        std::cout << "Successfully connected to the tracker!\n";
//
//        std::cout << "Decoded tracker response: \n";
//
//        return Decoder::decode_tracker_response(r.text);
//    } else {
//        std::cout << "Error  when connecting to the tracker: " << r.error.message << std::endl;
//
//        return {};
//    }

    std::string hex = "64383a636f6d706c657465693130346531303a646f776e6c6f616465646931346531303a696e636f6d706c65746569343965383a696e74657276616c69313830306531323a6d696e20696e74657276616c693138303065353a7065657273363a2e7558319a1f6565";

    return Decoder::decode_tracker_response(binaryStringDecoding(hex));
}

AnnounceRequest::AnnounceRequest(const torrent &torrent) {
    // kaladin == kl :/
    this->peer_id = "-kl0100-4auzKmHY8adc";
    this->base = torrent.getAnnounceUrl();

    this->left = torrent.getLength();
    this->info_hash = sha1_hash(torrent.getEncodedInfo());
}

std::string AnnounceRequest::sha1_hash(const std::string &value) {
    SHA1 checksum;

    checksum.update(value);

    return checksum.final();
}

std::vector<std::string> split(const std::string &s, const std::string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

std::vector<std::string> AnnounceRequest::baseURLGetter(const std::string &base) {
    if (base.find('?') == std::string::npos) {
        return std::vector{base};
    }

    std::string end = base.substr(base.find('?') + 1);

    auto splits = split(end, "=");

    return std::vector{base.substr(0, base.find('?')), splits[0], splits[1]};
}



