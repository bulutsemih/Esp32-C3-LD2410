#include "Degiskenler.h"
#include "bilesenler/AgBaglantisi.h"
#include "bilesenler/Ayarlar.h"
#include "bilesenler/RadarSensoru.h"
#include "bilesenler/WebSunucusu.h"
#include <Arduino.h>

void setup() {
  // Isinma Sorunu İcin: CPU Frekansini 80MHz'e sabitle (Varsayilan 160MHz)
  setCpuFrequencyMhz(80);

  // Başlangıç Gecikmesi
  delay(3000);

  // Görsel Kontrol (LED Test)
  pinMode(LED_PIN, OUTPUT);
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }

  Serial.begin(115200);
  delay(2000);

  // 1. Ayarları ve WiFi Bağlantısını Yükle
  ayarlariYukle();

  // 2. Radarı Başlat
  radarBaslat();

  // 3. Web Sunucusunu Başlat
  webSunucusuBaslat();
}

void loop() {
  // Radar Verilerini Oku ve Yönet
  radarDongu();

  // Web İsteklerini İşle
  server.handleClient();

  // MQTT İşlemleri
  mqttDongu();

  // Isinma icin biraz daha uzun bekleme (5ms -> 30ms)
  // Radar zaten 100ms'de bir veri gonderiyor, 30ms gayet guvenli.
  delay(30);

  // LED Heartbeat (Çalışıyor Sinyali)
  if ((millis() / 500) % 2 == 0) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}
