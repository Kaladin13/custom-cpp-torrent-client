//
// Created by kaladin on 27.06.23.
//

#ifndef TORRENT_PROTOCOL_UTILITY_H
#define TORRENT_PROTOCOL_UTILITY_H

#include <string>

std::string binaryStringDecoding(const std::string &value) {
    int hashLength = value.length();

    std::string decodedHexString;

    for (int i = 0; i < hashLength; i += 2) {
        std::string byte = value.substr(i, 2);
        char c = (char) (int) strtol(byte.c_str(), nullptr, 16);
        decodedHexString.push_back(c);
    }

    return decodedHexString;
}

#endif //TORRENT_PROTOCOL_UTILITY_H
