#include "utils.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// 모니터 모드 자동 실행
void start_monitor_mode(char *interface) {
    char command[100];
    sprintf(command, "sudo gmon %s", interface);
    system(command);
}

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