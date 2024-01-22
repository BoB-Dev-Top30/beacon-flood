// 비콘프레임 생성 및 전송모듈
#include "beacon_frame.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib> 
#include <vector>
#include <pcap.h>
#include <chrono>
#include <thread>
// 비콘프레임 추출

bool Distinguish_Beacon(const uint8_t *packet) {
    radiotap_header *radio_hdr = (radiotap_header *)packet;
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


// SSID 위치 찾기
int find_ssid_position(const uint8_t *packet, int packet_len) {

    radiotap_header *radio_hdr = (radiotap_header *)packet;
    int radio_len = radio_hdr->len;
    int beacon_len = 24;
    int wireless_static_len = 12;

    int offset = radio_len + beacon_len + wireless_static_len;
    
    printf("%d this is offset\n", offset);
    return offset;
}

// SSID의 이름 반환(테스트용도)
uint8_t * find_wireless_static(const uint8_t *packet, int *ssid_length) 
{
    radiotap_header *radio_hdr = (radiotap_header *)packet;
    int offset = radio_hdr->len;
    
    printf("static에서 찾은 offset %d\n", offset);
    Tag_SSID * SSID = (Tag_SSID *)(packet + offset + 24 + 12); // 24는 비콘프레임의 fix값
    printf("static에서 찾은 offset %d\n", offset);
    printf("static에서 찾은 ssid 길이: %d\n", SSID->tag_length);
    uint8_t *ssid = SSID->ssid;
    *ssid_length = SSID->tag_length; //포인터로 저장

    return ssid;
}


uint8_t* modify_beacon_ssid(const uint8_t *packet, int packet_len, const char* new_ssid) {
    
    // 패킷 복사
    uint8_t *new_packet = (uint8_t *)malloc(packet_len);
    if (!new_packet) return NULL;
    memcpy(new_packet, packet, packet_len);

    // SSID 위치찾기
    int ssid_pos = find_ssid_position(new_packet, packet_len);

    if (ssid_pos >= 0) {
        int ssid_len = strlen(new_ssid); // 널문자 포함하지만 어차피 memcpy할때-1 뺴야함

        new_packet[ssid_pos + 1] = ssid_len; // SSID 길이 업데이트(첫값은 시그니처값이기에 +1한다!)
        memcpy(new_packet + ssid_pos + 2, new_ssid, ssid_len); // (+2해서진짜위치부터 memcpy한다.)

    }
    return new_packet;
}



void send_packet(pcap_t* handle, const uint8_t* packet, int length) {
    // 지속적으로 패킷 전송
    auto start = std::chrono::high_resolution_clock::now(); // 현재 시간을 start 변수에 저장

    while (true) {
        auto end = std::chrono::high_resolution_clock::now(); // 현재 시간을 end 변수에 저장
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []{ return ready; });
        }
        pcap_sendpacket(handle, packet, length);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 1초에 천번
        /*
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        if (elapsed.count() > 30) {
            break; // 30초 이상 지나면 반복문 탈출
        }
        */
    }
}