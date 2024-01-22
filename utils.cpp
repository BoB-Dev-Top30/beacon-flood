#include "utils.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>

// 모니터 모드 자동 실행
void start_monitor_mode(char *interface) {
    char command[100];
    sprintf(command, "sudo gmon %s", interface);
    system(command);
}

// 벡터로 파일 읽어오기
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

void change_channel(const char* interface, int channel) {
    char command[100];
    sprintf(command, "iwconfig %s channel %d", interface, channel);
    system(command);
}
// 쓰려고 했는데 굳이 필요없었음
const char* generate_random_mac_address() {
    srand(time(0));

    static char mac[18];
    unsigned char mac_bytes[6];

    mac_bytes[0] = 0x02 | (rand() & 0xFC);  // 첫 번째 바이트는 특정 비트가 0이어야 함
    for(int i = 1; i < 6; i++) {
        mac_bytes[i] = rand() % 256;
    }

    sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", mac_bytes[0], mac_bytes[1], mac_bytes[2], mac_bytes[3], mac_bytes[4], mac_bytes[5]);

    return mac;
}