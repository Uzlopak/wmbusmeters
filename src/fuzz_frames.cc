/*
 Copyright (C) 2019-2026 Fredrik Öhrström (gpl-3.0-or-later)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include"util.h"
#include"wmbus.h"

#include<unistd.h>
#include<vector>

using namespace std;

int main(int argc, char **argv)
{
    char buf[4096];
    vector<uchar> data;
    vector<char> *ptr = reinterpret_cast<vector<char>*>(&data);

    if (argc > 1 && argv[1][0] != 0)
    {
        loadFile(string(argv[1]), ptr);
    }
    else
    {
        for (;;) {
            ssize_t len = read(0, buf, sizeof(buf));
            if (len <= 0) break;
            data.insert(data.end(), buf, buf + len);
        }
    }

    size_t frame_length = 0;
    int payload_len = 0;
    int payload_offset = 0;

    // Fuzz the WMBus frame validator (format A/B, CRC trimming, sync checks).
    {
        vector<uchar> copy = data;
        checkWMBusFrame(copy, &frame_length, &payload_len, &payload_offset, true);
    }

    // Fuzz the MBus frame validator (start/stop bytes, checksum, short/long frame).
    {
        vector<uchar> copy = data;
        checkMBusFrame(copy, &frame_length, &payload_len, &payload_offset, true);
    }

    return 0;
}
