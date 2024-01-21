// 비콘프레임 생성 및 전송모듈
// #include "beacon_frame.h"
// 비콘프레임 추출

/*
int extract_beacon(const struct pcap_pkthdr *header, const u_char *packet) {
    struct radiotap_header *radio_hdr = (struct radiotap_header *)packet;
    uint16_t radiotap_header_length = radio_hdr->len;
    u_char type_subtype = packet[radiotap_header_length];
    u_char type = (type_subtype & 0x0C) >> 2;    // 타입 필드
    u_char subtype = (type_subtype & 0xF0) >> 4; // 서브타입 필드
    if (type == 0 && subtype == 8) {
        printf("Beacon Frame Captured\n");
        return 1;
    }
    else{
        return 0;
    }
}

//비콘프레임 + 필요한 데이터 받아서 전송
bool send_beacon(){

    return true;
}
*/