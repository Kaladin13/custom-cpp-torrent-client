//
// Created by kaladin on 21.06.23.
//

#include "torrent.h"

torrent::torrent() = default;

const std::string &torrent::getAnnounceUrl() const {
    return announce_URL;
}

void torrent::setAnnounceUrl(const std::string &announceUrl) {
    announce_URL = announceUrl;
}

const std::string &torrent::getFilename() const {
    return filename;
}

void torrent::setFilename(const std::string &filename) {
    torrent::filename = filename;
}

bool torrent::isPrivate() const {
    return is_private;
}

void torrent::setIsPrivate(bool isPrivate) {
    is_private = isPrivate;
}

int torrent::getPieceLength() const {
    return piece_length;
}

void torrent::setPieceLength(int pieceLength) {
    piece_length = pieceLength;
}

const std::string &torrent::getPieces() const {
    return pieces;
}

void torrent::setPieces(const std::string &pieces) {
    torrent::pieces = pieces;
}

const std::string &torrent::getEncodedInfo() const {
    return encoded_info;
}

void torrent::setEncodedInfo(const std::string &encodedInfo) {
    encoded_info = encodedInfo;
}

long long int torrent::getLength() const {
    return length;
}

void torrent::setLength(long long int length) {
    torrent::length = length;
}
