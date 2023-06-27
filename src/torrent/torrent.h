//
// Created by kaladin on 21.06.23.
//

#ifndef TORRENT_PROTOCOL_TORRENT_H
#define TORRENT_PROTOCOL_TORRENT_H


#include <string>

class torrent {
    std::string announce_URL;
    std::string filename;
    bool is_private{};

    int piece_length{};
    long long length{};
    std::string pieces;
    std::string encoded_info;
public:
    torrent();

    const std::string &getEncodedInfo() const;

    void setEncodedInfo(const std::string &encodedInfo);

    const std::string &getAnnounceUrl() const;

    void setAnnounceUrl(const std::string &announceUrl);

    const std::string &getFilename() const;

    void setFilename(const std::string &filename);

    bool isPrivate() const;

    void setIsPrivate(bool isPrivate);

    int getPieceLength() const;

    void setPieceLength(int pieceLength);

    const std::string &getPieces() const;

    void setPieces(const std::string &pieces);

    long long int getLength() const;

    void setLength(long long int length);

    inline static std::string pub_encoded_info;
};


#endif //TORRENT_PROTOCOL_TORRENT_H
