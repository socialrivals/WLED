#pragma once

#include "wled.h"
#include "Arduino.h"

#include <deque>

#define USERMOD_BROADCAST_INFO_VERSION "1"

/*
 * Usermods allow you to add own functionality to WLED more easily
 * See: https://github.com/Aircoookie/WLED/wiki/Add-own-functionality
 * 
 * Using a usermod:
 * 1. Copy the usermod into the sketch folder (same folder as wled00.ino)
 * 2. Register the usermod by adding #include "usermod_filename.h" in the top and registerUsermod(new MyUsermodClass()) in the bottom of usermods_list.cpp
 */

class BroadcastInfoUsermod : public Usermod {
  private:
    unsigned long lastTime_ = 0;
    unsigned long interval_ = 3000; // default 3s until uptime > 30s
    bool started_ = false;

    String sno;
    
    WiFiUDP udp;
    const uint16_t UDP_PORT = 11345;
    IPAddress broadcastIp;

    void broadcastMessage(const char* msg) {
      udp.beginPacket(Network.broadcastIP(), UDP_PORT);
      udp.write((const uint8_t*)msg, strlen(msg));
      udp.endPacket();
      DEBUG_PRINTLN(F("[Broadcast] Message broadcasted."));
    }
  public:
    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */
    void setup() {
      lastTime_ = millis();
      sno = WiFi.macAddress();
      sno.replace(":", "");
      sno.toUpperCase();
    }

    /*
     * loop() is called continuously. Here you can check for events, read sensors, etc.
     */
    void loop() {
      unsigned long now = millis();

      // Calculate uptime in seconds
      unsigned long uptime = now / 1000;

      if (now - lastTime_ < interval_) return;

      // Update broadcast interval
      if (uptime >= 30) {
        interval_ = 10000 + random(0, 5000);
      } else {
        interval_ = 3000;               // 3s before 30s uptime
      }

      lastTime_ = now;

      // Start broadcasting only after uptime exceeds 30s
      if (Network.isConnected()) {
        // MSG Format: VERSION;SNO;MAC;UPTIME;TYPE
        String msg = String(USERMOD_BROADCAST_INFO_VERSION) + ";" +
                     sno + ";" +
                     Network.localMAC() + ";" +
                     String(uptime) + ";" +
                     String(F("esp32"));
        broadcastMessage(msg.c_str());
      }
    }
};