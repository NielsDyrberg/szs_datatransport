//
// Created by ncpd on 28-11-2021.
//

#ifndef SZS_DATATRANSPORT_SHARED_TYPES_H
#define SZS_DATATRANSPORT_SHARED_TYPES_H

#include <cstring>
#include <string>

#include <cstdint>

#define NEW_FILE (uint8_t) 0xCC
#define PACKET (uint8_t) 0xDD
#define FILE_END (uint8_t) 0xEE

int block_size[] = {4096, 2048, 1024, 512, 256, 128, 64};
std::string block_size_char[] = {"4096", "2048", "1024", "512", "256", "128", "64"};
const int n_block_size = sizeof(block_size)/ sizeof(int);

int send_delay[] = {0, 20, 50, 70, 100, 130};
std::string send_delay_char[] = {"0", "20", "50", "70", "100", "130"};
const int n_send_delay = sizeof(send_delay)/ sizeof(int);

std::string test_files_path = "/tmp/szs_datatransport/tests/test_datatransport_meas_report_qos_opt/test_files/";
std::string rcv_files_path = "/tmp/";

enum cid_t : uint8_t {
    package_start = 0x01,
    package_end = 0x02,
};

#endif //SZS_DATATRANSPORT_SHARED_TYPES_H
