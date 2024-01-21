// 비콘프레임 생성 및 전송모듈
#include "beacon_frame.h"
#include <cstdint>
#include <cstdio>
// 비콘프레임 추출


bool Distinguish_Beacon(const struct pcap_pkthdr *header, const uint8_t *packet) {
    struct radiotap_header *radio_hdr = (struct radiotap_header *)packet;
    uint16_t radiotap_header_length = radio_hdr->len;
    uint8_t type_subtype = packet[radiotap_header_length];
    uint8_t type = (type_subtype & 0x0C) >> 2;    // 타입 필드
    uint8_t subtype = (type_subtype & 0xF0) >> 4; // 서브타입 필드
    if (type == 0 && subtype == 8) {
        printf("Beacon Frame Captured\n");
        return true;
    }
    else{
        return false;
    }
}

/*
//비콘프레임 + 필요한 데이터 받아서 전송
bool send_beacon(){

    return true;
}
*/