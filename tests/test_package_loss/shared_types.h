//
// Created by ncpd on 19-11-2021.
//

#ifndef SZS_DATATRANSPORT_SHARED_TYPES_H
#define SZS_DATATRANSPORT_SHARED_TYPES_H

#include <cstdint>

enum cid_t : uint8_t {
    package_start = 0x01,
    package_end = 0x02,
};

#endif //SZS_DATATRANSPORT_SHARED_TYPES_H
