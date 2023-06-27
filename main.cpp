#include <fstream>
#include <cpr/cpr.h>
#include "src/decoder/Decoder.h"
#include "src/requests/AnnounceRequest.h"

using namespace std;

int main() {
    Decoder decoder(
            "../public/test-torrents/Riddik.EC.2013.RUS.BDRip.XviD.AC3.avi [piratetorrent.ru].torrent");

    torrent metainfo = decoder.decode_as_torrent();
    AnnounceRequest announceRequest(metainfo);

    PeersManager peersManager = announceRequest.make_tracker_response();
    peersManager.print_peers();

    peersManager.fetch_peers();

    return 0;
}
