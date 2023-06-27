cmake_minimum_required(VERSION 3.23)
project(torrent_protocol)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

include_directories(
        "include" "vendor"
)

add_executable(torrent_protocol main.cpp src/decoder/Decoder.cpp src/decoder/Decoder.h src/torrent/torrent.cpp src/torrent/torrent.h src/requests/AnnounceRequest.cpp src/requests/AnnounceRequest.h src/peers/PeersManager.cpp src/peers/PeersManager.h src/peers/Peer.cpp src/peers/Peer.h src/decoder/utility.h src/net/socket_data.h)

target_link_libraries(torrent_protocol PRIVATE cpr::cpr)
