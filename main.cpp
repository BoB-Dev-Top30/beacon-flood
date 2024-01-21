//메인 파일 
// SSID 불러와서 BeaconFrame 계속 전송

#include <unistd.h>
#include <pcap.h>
#include <iostream>
#include <chrono> // 시간측정위한것

#include "beacon_frame.h"

#include "utils.h" //파일 및 모니터모드 관련


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

  // gmon 설치 필요!
  start_monitor_mode(argv[1]);

  while (1) 
  {
    handle = pcap_open_live(argv[1], BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) 
    {
      fprintf(stderr, "Couldn' Capture': %s\n", errbuf);
        return 0;
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

    int Is_Beacon = Distinguish_Beacon(header, packet);

    if (Is_Beacon==1) 
    {
      printf("This IS Beacon\n");
      /*
      u_char *modified_packet = modify_beacon_ssid(packet, header->caplen, "New_SSID"); // 변경된 패킷 재전송
      if (modified_packet != NULL) 
        {
          while(1)
          {
            auto end = std::chrono::high_resolution_clock::now();
            pcap_sendpacket(handle, modified_packet, header->caplen);
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
            if(elapsed.count()>9){break;}
          }
          free(modified_packet);
        }
      */
    }
  }
}