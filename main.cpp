//메인 파일 
// SSID 불러와서 BeaconFrame 계속 전송

#include <unistd.h>
#include <pcap.h>
#include <iostream>
#include <chrono> // 시간측정위한것
#include <cstring>
#include <vector>
#include "beacon_frame.h" // 비콘프레임 관련

#include "utils.h" //파일 및 모니터모드 관련

#include <thread> // 동시전송을 위한 쓰레드


#define SSID_MAX_LEN 32 //ssid의 최대길이

std::mutex mtx;
std::condition_variable cv;
bool ready = false;


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

    int Is_Beacon = Distinguish_Beacon(packet);
    
    std::vector<uint8_t*> modified_packets; // 변경된 패킷 저장을 위한 벡터
    if (Is_Beacon==1) 
    {
      printf("This IS Beacon\n");
      printf("%d\n", header->caplen); //길이 테스트출력
      
      for (const auto& ssid : ssids) 
      {
        uint8_t *modified_packet = modify_beacon_ssid(packet, header->caplen, ssid.c_str()); // 벡터에 저장된 ssid만큼 변경된 비콘 프레임생성
        if (modified_packet == NULL) {
          // 오류 처리: 메모리 할당 실패 또는 다른 이유
          printf("modified_packet 메모리 할당 실패!\n");
          continue; // 다음 SSID로 넘김
        }
        modified_packets.push_back(modified_packet);

        // 테스트용 출력 코드
        int ssid_length;
        
        // <uint8_t>로변환
        uint8_t *ssid_position = find_wireless_static(modified_packet, &ssid_length);
        if (ssid_position == NULL || ssid_length > SSID_MAX_LEN) {
          // 오류 처리: 유효하지 않은 SSID 위치 또는 길이
          printf("테스트 ssid 메모리 할당 실패!\n");
          free(modified_packet);
          continue; // 다음 SSID로 넘어갑니다
        }

      char modified_ssid[SSID_MAX_LEN + 1];
      memcpy(modified_ssid, ssid_position, ssid_length);
      modified_ssid[ssid_length] = '\0';
      printf("Modified SSID: %s\n", modified_ssid);
      }

      
      // 각 변경된 패킷에 대해 별도의 스레드 생성하여 전송
      std::vector<std::thread> threads;
      for (auto it = modified_packets.begin(); it != modified_packets.end();)
      {
        for (int i = 0; i < 10 && it != modified_packets.end(); ++i, ++it) // 10개만 일단은
        {
          threads.push_back(std::thread(send_packet, handle, *it, header->caplen));
        }

        {
          std::unique_lock<std::mutex> lock(mtx);
          ready = true;
          cv.notify_all(); // 모든 스레드에 시작 신호 전달
        }

    // 모든 스레드가 완료될 때까지 기다림
        for (auto& thread : threads) 
        {
          thread.join();
        }
        threads.clear();
      }
    }
     for (auto& modified_packet : modified_packets)
      {
        free(modified_packet);  // 잡은 패킷으로 한번 쭉 전송하고 메모리해제
      }
  }
    
}

