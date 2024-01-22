#include <string>
#include <vector>
#include <iostream>
#include <fstream>

void start_monitor_mode(char *interface);

std::vector<std::string> read_ssids_from_file(const std::string& filename);
void change_channel(const char* interface, int channel);
const char* generate_random_mac_address();