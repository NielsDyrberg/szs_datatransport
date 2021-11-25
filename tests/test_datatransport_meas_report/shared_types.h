//
// Created by ncpd on 19-11-2021.
//

#ifndef SZS_DATATRANSPORT_SHARED_TYPES_H
#define SZS_DATATRANSPORT_SHARED_TYPES_H

#include <cstdint>

#define PING_REQUEST (uint8_t) 0xAA
#define PING_RESPONSE (uint8_t) 0xBB

enum cid_t : uint8_t {
    package_start = 0x01,
    package_end = 0x02,
};

#endif //SZS_DATATRANSPORT_SHARED_TYPES_H
