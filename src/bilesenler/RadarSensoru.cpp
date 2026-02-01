#include "RadarSensoru.h"

/**
 * Radar Modülüne Mühendislik Modu Komutlarını Gönderir
 */
// Kalibrasyon Değişkenleri
bool kalibrasyon_aktif = false;
unsigned long kalibrasyon_baslangic = 0;
int max_hareket_enerjisi[9] = {0};
int max_duragan_enerji[9] = {0};
bool kalibrasyon_bitti = false;

/**
 * Radar Modülüne Mühendislik Modu Komutlarını Gönderir
 */
void forceEngineeringMode() {
  // Serial Buffer'ı temizle
  while (RadarSerial.available()) {
    RadarSerial.read();
  }

  // 1. Config Mode Gir
  uint8_t cmdEnterConfig[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xFF,
                              0x00, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
  RadarSerial.write(cmdEnterConfig, sizeof(cmdEnterConfig));
  RadarSerial.flush();
  delay(100);

  while (RadarSerial.available()) {
    RadarSerial.read();
  }

  // 2. Engineering Mode Başlat
  uint8_t cmdStartEng[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00,
                           0x62, 0x00, 0x04, 0x03, 0x02, 0x01};
  RadarSerial.write(cmdStartEng, sizeof(cmdStartEng));
  RadarSerial.flush();
  delay(100);

  while (RadarSerial.available()) {
    RadarSerial.read();
  }

  // 3. Config Mode Çık
  uint8_t cmdEndConfig[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00,
                            0xFE, 0x00, 0x04, 0x03, 0x02, 0x01};
  RadarSerial.write(cmdEndConfig, sizeof(cmdEndConfig));
  RadarSerial.flush();
  delay(100);
}

/**
 * Radar Başlangıç Ayarları
 */
void radarBaslat() {
  // ESP32-C3 SuperMini Seri Port Başlatma (GPIO 6 & 7)
  RadarSerial.begin(256000, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(500);

  radar.begin(RadarSerial);
  delay(500);

  // Radar bağlantı kontrolü
  unsigned long startWait = millis();
  while (millis() - startWait < 3000) {
    radar.read();
    if (radar.isConnected())
      break;
    delay(10);
  }

  if (radar.isConnected()) {
    Serial.println("✓ Radar Bağlantısı Başarılı.");
    radar.requestCurrentConfiguration();
    // Gate 0 (0-75cm) DONANIMSAL KÖRLEME
    radar.setGateSensitivityThreshold(0, 100, 100);
  } else {
    Serial.println("✗ Radar Bağlantı Hatası!");
  }

  // Mühendislik modunu aktif etme denemesi
  Serial.println("Mühendislik modu zorlanıyor...");
  for (int i = 0; i < 3; i++) {
    forceEngineeringMode();
    for (int j = 0; j < 50; j++) {
      radar.read();
      delay(10);
      if (ld2410_last_frame_type == 0x01) {
        Serial.println("✓ Mühendislik modu AKTİF.");
        return;
      }
    }
    delay(200);
  }
}

/**
 * Ana Radar Döngüsü ve Karar Motoru
 */
void radarDongu() {
  radar.read();

  if (radar.isConnected()) {

    // --- DİNAMİK KARAR MOTORU (Geliştirilmiş AnKA-GLC Mantığı) ---
    // Hareketli VEYA Durağan hedef, belirtilen mesafe ve enerji eşiklerini
    // sağlıyor mu?

    // 1. Hareketli Hedef Kontrolü
    bool hareketli_uygun = false;
    // Gate 0 (0-75cm) gürültüsünü önlemek için > 75cm şartı eklendi
    if (radar.movingTargetDistance() > 75 &&
        radar.movingTargetDistance() <= hareket_mesafe_esigi) {
      if (radar.movingTargetEnergy() >= hareket_enerji_esigi) {
        hareketli_uygun = true;
      }
    }

    // 2. Durağan Hedef Kontrolü
    bool duragan_uygun = false;
    // Gate 0 (0-75cm) gürültüsünü önlemek için > 75cm şartı eklendi
    if (radar.stationaryTargetDistance() > 75 &&
        radar.stationaryTargetDistance() <= duragan_mesafe_esigi) {
      if (radar.stationaryTargetEnergy() >= duragan_enerji_esigi) {
        duragan_uygun = true;
      }
    }

    // Sonuç: İkisinden biri varsa VARLIK VARDIR.
    if (hareketli_uygun || duragan_uygun) {
      insan_varligi = true;
      son_varlik_zamani = millis(); // Varlık olduğu sürece zamanı güncelle
      if (led_alarm_modu) {
        digitalWrite(LED_PIN, HIGH);
      }
    } else {
      // Varlık sinyali kesilse bile hemen kapatma (Debounce / Soğuma Süresi)
      // Son varlıktan 2 saniye (2000 ms) sonra kapat
      if (millis() - son_varlik_zamani > 2000) {
        insan_varligi = false;
        digitalWrite(LED_PIN, LOW);
      }
    }

    // DEBUG LOGLAMA (Her 500ms'de bir)
    static unsigned long lastDebug = 0;
    if (millis() - lastDebug > 500) {
      lastDebug = millis();
      Serial.print("Hareketli: Dist=");
      Serial.print(radar.movingTargetDistance());
      Serial.print(" Eng=");
      Serial.print(radar.movingTargetEnergy());
      Serial.print(" | Duragan: Dist=");
      Serial.print(radar.stationaryTargetDistance());
      Serial.print(" Eng=");
      Serial.print(radar.stationaryTargetEnergy());
      Serial.print(" | M_Esik: ");
      Serial.print(hareket_mesafe_esigi);
      Serial.print("/");
      Serial.print(hareket_enerji_esigi);
      Serial.print(" | S_Esik: ");
      Serial.print(duragan_mesafe_esigi);
      Serial.print("/");
      Serial.print(duragan_enerji_esigi);
      Serial.print(" | Karar: ");
      Serial.println(insan_varligi ? "EVET" : "HAYIR");
    }
    // ----------------------------------------------

    // Otomatik Mod Kontrolü (Auto-Heal)
    // Eğer modül temel moda düştüyse (0x02) mühendislik moduna geri döndürür
    if (millis() - lastEngineeringCheck > 5000) {
      lastEngineeringCheck = millis();

      bool hasEngineeringData = false;
      for (int i = 0; i < 9; i++) {
        if (radar.gate_moving_energy[i] > 0 ||
            radar.gate_stationary_energy[i] > 0) {
          hasEngineeringData = true;
          break;
        }
      }

      if (ld2410_last_frame_type == 0x02 && !hasEngineeringData) {
        Serial.println("⚠ Modül temel moda düştü, düzeltiliyor...");
        forceEngineeringMode();
      }
    }

    // --- KALİBRASYON MANTIĞI ---
    if (kalibrasyon_aktif) {
      // Veri Topla (5 Saniye Boyunca)
      if (millis() - kalibrasyon_baslangic < 5000) {
        for (int i = 0; i < 9; i++) {
          if (radar.gate_moving_energy[i] > max_hareket_enerjisi[i]) {
            max_hareket_enerjisi[i] = radar.gate_moving_energy[i];
          }
          if (radar.gate_stationary_energy[i] > max_duragan_enerji[i]) {
            max_duragan_enerji[i] = radar.gate_stationary_energy[i];
          }
        }
      } else {
        // Süre Doldu -> Yeni Eşikleri Uygula
        Serial.println("✓ Kalibrasyon Veri Toplama Tamamlandı. Yeni Eşikler "
                       "Uygulanıyor...");

        // 0-8 arası tüm kapılar için güncelle
        for (int i = 0; i < 9; i++) {
          // Yeni Eşik = Ölçülen Max + 15 (Güvenlik Payı)
          int yeni_hareket_esik = max_hareket_enerjisi[i] + 15;
          int yeni_duragan_esik = max_duragan_enerji[i] + 15;

          // Gate 0 (0-75cm) DONANIMSAL KÖRLEME
          if (i == 0) {
            yeni_hareket_esik = 100;
            yeni_duragan_esik = 100;
          }

          // Güvenlik sınırları (Min 20, Max 100)
          if (yeni_hareket_esik < 20)
            yeni_hareket_esik = 20;
          if (yeni_hareket_esik > 100)
            yeni_hareket_esik = 100;

          if (yeni_duragan_esik < 20)
            yeni_duragan_esik = 20;
          if (yeni_duragan_esik > 100)
            yeni_duragan_esik = 100;

          // Gate Sensitivity Ayarla
          radar.setGateSensitivityThreshold(i, yeni_hareket_esik,
                                            yeni_duragan_esik);
          delay(50); // Sensöre yazma arası bekleme

          Serial.printf("Gate %d -> M:%d S:%d\n", i, yeni_hareket_esik,
                        yeni_duragan_esik);
        }

        kalibrasyon_aktif = false;
        kalibrasyon_bitti = true;
        radar.requestCurrentConfiguration(); // Yeni değerleri hafızaya al
        Serial.println("✓ Kalibrasyon BAŞARIYLA Tamamlandı.");
      }
    }

  } else {
    // Bağlantı koptuysa yeniden başlatma denemesi
    if (millis() - lastRadarFixAttempt > 10000) {
      lastRadarFixAttempt = millis();
      Serial.println("⚠ Radar bağlantısı koptu, yeniden deneniyor...");
      radar.begin(RadarSerial);
    }
  }
}

void kalibrasyonBaslat() {
  kalibrasyon_aktif = true;
  kalibrasyon_baslangic = millis();
  kalibrasyon_bitti = false;

  // Dizileri sıfırla
  for (int i = 0; i < 9; i++) {
    max_hareket_enerjisi[i] = 0;
    max_duragan_enerji[i] = 0;
  }

  // Mühendislik moduna zorla (Veri toplamak için)
  forceEngineeringMode();
  Serial.println("KALİBRASYON BAŞLATILDI: 5 Saniye Veri Toplanacak...");
}

String kalibrasyonDurumu() {
  if (kalibrasyon_aktif)
    return "CALIBRATING";
  if (kalibrasyon_bitti)
    return "DONE";
  return "IDLE";
}