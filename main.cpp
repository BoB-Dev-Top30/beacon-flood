//메인 파일 
// SSID 불러와서 BeaconFrame 계속 전송

#include <unistd.h>
#include <list>
#include <tins/tins.h>

using namespace std;
using namespace Tins;

int main()
{
  list<string> ssidList {
    "1.길길이",
    "2.엄청잘생김",
    "3.서울에서제일잘생김",
    "4.한국에서제일잘생김",
    "5.세상에서제일잘생김",
    "6.완전짱인듯"
  };
  list<string>::iterator it = ssidList.begin();
  while (true) {
    RadioTap tap;

    Dot11::address_type ap        = "00:11:22:33:44:55";
    Dot11::address_type broadcast = "ff:ff:ff:ff:ff:ff";
    Dot11Beacon beacon(broadcast, ap);
    beacon.addr4(ap);
    beacon.ssid(*it);
    beacon.ds_parameter_set(1);
    beacon.supported_rates({ 1.0f, 5.5f, 11.0f });
    tap.inner_pdu(beacon);

    PacketSender sender("wlan0");
    sender.send(tap);
    usleep(1000);

    if (++it == ssidList.end())
      it = ssidList.begin();
  }
}