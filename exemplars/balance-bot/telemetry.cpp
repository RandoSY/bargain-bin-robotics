#include "telemetry.h"

#include <Arduino.h>
#include <stdio.h>
#include <WiFi.h>
#include <WiFiUdp.h>

namespace {
constexpr char kApSsid[] = "Bala2-Telemetry";
constexpr char kApPassword[] = "bala2log";
constexpr uint16_t kUdpPort = 4210;
const IPAddress kBroadcastAddress(192, 168, 4, 255);

WiFiUDP telemetry_udp;
bool telemetry_ready = false;
}  // namespace

void telemetryBegin() {
  WiFi.mode(WIFI_AP);
  WiFi.setSleep(false);

  if (!WiFi.softAP(kApSsid, kApPassword)) {
    Serial.println("Telemetry Wi-Fi AP failed");
    return;
  }

  if (!telemetry_udp.begin(kUdpPort)) {
    Serial.println("Telemetry UDP setup failed");
    return;
  }

  telemetry_ready = true;
  Serial.printf("Telemetry AP: %s  password: %s  UDP: %u\n",
                kApSsid,
                kApPassword,
                static_cast<unsigned>(kUdpPort));
  Serial.printf("Telemetry IP: %s\n", WiFi.softAPIP().toString().c_str());
}

void telemetrySend(uint32_t timestamp_ms,
                   float angle,
                   float angle_rate,
                   float target_angle,
                   float motor_speed,
                   int16_t pwm_angle,
                   int16_t pwm_speed,
                   int16_t pwm_output,
                   int32_t encoder_left,
                   int32_t encoder_right) {
  if (!telemetry_ready) {
    return;
  }

  char packet[192];
  int length = snprintf(packet,
                        sizeof(packet),
                        "%lu,%.4f,%.4f,%.4f,%.4f,%d,%d,%d,%ld,%ld\n",
                        static_cast<unsigned long>(timestamp_ms),
                        angle,
                        angle_rate,
                        target_angle,
                        motor_speed,
                        pwm_angle,
                        pwm_speed,
                        pwm_output,
                        static_cast<long>(encoder_left),
                        static_cast<long>(encoder_right));
  if (length <= 0 || length >= static_cast<int>(sizeof(packet))) {
    return;
  }

  telemetry_udp.beginPacket(kBroadcastAddress, kUdpPort);
  telemetry_udp.write(reinterpret_cast<const uint8_t *>(packet), length);
  telemetry_udp.endPacket();
}
