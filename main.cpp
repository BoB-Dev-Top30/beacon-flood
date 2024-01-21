//메인 파일 
// SSID 불러와서 BeaconFrame 계속 전송

#include <unistd.h>
#include <pcap.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
/*
// 모니터 모드 자동 실행
void start_monitor_mode(char *interface) {
    char command[100];

    sprintf(command, "sudo gmon %s", interface);
    system(command);
}

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

std::vector<std::string> read_ssids_from_file(const std::string& filename) {
    std::vector<std::string> ssids;
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            ssids.push_back(line);
        }
        file.close();
    } else {
        std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
    }

    return ssids;
}


int main(int argc, char *argv[])
{
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t *handle;

  if (argc != 3) {
      fprintf(stderr, "Usage: %s <interface> or <list.txt>\n", argv[0]);
      return 2;
  }
  std::vector<std::string> ssids = read_ssids_from_file(argv[2]);

  // 읽은 SSID 출력 (테스트용)
  for (const auto& ssid : ssids) {
      std::cout << ssid << std::endl;
  }

  /*

  
  start_monitor_mode(argv[1]);

  while (1) 
  {
    handle = pcap_open_live(argv[1], BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) 
    {
      fprintf(stderr, "Couldn' Capture': %s\n", errbuf);
        return 2;
    }
    const u_char *packet;
    struct pcap_pkthdr *header;
    
    int res = pcap_next_ex(handle, &header, &packet);
        
    if (res == 0) {continue;} // 타임아웃 발생
    if (res == -1 || res == -2) 
    {
      fprintf(stderr, "End of pcap file or  pcap_next_ex failed: %s\n", pcap_geterr(handle));
      pcap_close(handle);
      break;
    }
    int Is_Beacon = process_packet(header, packet);
    if (Is_Beacon==1) 
    {
      u_char *modified_packet = modify_beacon_ssid(packet, header->caplen, "New_SSID"); // 변경된 패킷 재전송
        if (modified_packet != NULL) 
        {
          pcap_sendpacket(handle, modified_packet, header->caplen);
          free(modified_packet);
        }
    }
  }
  */

}