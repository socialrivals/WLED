#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else // ESP32
  #include <WiFi.h>
  #include <ETH.h>
#endif

#ifndef Network_h
#define Network_h

class NetworkClass
{
public:
  IPAddress localIP();
  String localIPString();
  IPAddress subnetMask();
  IPAddress gatewayIP();
  IPAddress broadcastIP();
  void localMAC(uint8_t* MAC);
  String localMAC();
  bool isConnected();
  bool isEthernet();
};

extern NetworkClass Network;

#endif