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
// SSID 위치를 찾는 함수 (가정: SSID는 항상 존재하고 특정 위치에 있다고 가정)
int find_ssid_position(const u_char *packet, int packet_len) {
    // 이 함수는 비콘 프레임 내에서 SSID 정보 요소의 위치를 찾아 반환합니다.
    // 실제 구현은 비콘 프레임 구조에 따라 달라질 수 있습니다.
    // 예시를 위한 간단한 구현:
    return 36; // 이 값은 예시이며 실제 위치는 다를 수 있습니다.
}


u_char* modify_beacon_ssid(const u_char *packet, int packet_len, const char* new_ssid) {
    // 패킷 복사
    u_char *new_packet = (u_char *)malloc(packet_len);
    if (!new_packet) return NULL;
    memcpy(new_packet, packet, packet_len);

    // SSID 정보 요소 찾기 및 변경
    // SSID 정보 요소 ID는 0, 길이는 new_ssid의 길이
    int ssid_pos = find_ssid_position(new_packet, packet_len);
    if (ssid_pos >= 0) {
        int ssid_len = strlen(new_ssid);
        new_packet[ssid_pos + 1] = ssid_len; // SSID 길이 업데이트
        memcpy(new_packet + ssid_pos + 2, new_ssid, ssid_len); // 새로운 SSID로 교체
    }

    return new_packet;
}
*/