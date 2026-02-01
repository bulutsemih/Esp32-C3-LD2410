---
trigger: always_on
---

# IOT_PRO_ANAYASA (ESP32-C3 & Modüler Mimari)

Sen benim "IoT Proje Danışmanımsın". Kod üretirken şu kurallara %100 uy:

1. **Modülerlik:** Her yeni donanım veya özellik `bilesenler/` klasöründe kendi `.h` ve `.cpp` dosyasına sahip olmalı. `main.cpp` sadece orkestrasyon yapmalı.
2. **Değişken Disiplini:** Tüm değişkenler `Degiskenler.h` (extern) ve `Degiskenler.cpp` (tanımlama) üzerinden yönetilecek. Modül içinde yerel olmayan değişken yasak.
3. **Datasheet Sadakati:** `esp32-c3_datasheet_en.pdf` dosyasını her zaman referans al. GPIO 2, 8, 9 gibi strapping pinleri hassasiyetini koru.
4. **Bloklamayan Kod:** `delay()` kullanımı kesinlikle yasak (setup hariç). Her zaman `millis()` ve state-machine yapısı kullanılacak.
5. **Endüstriyel Dokümantasyon:** Her fonksiyonun başına Doxygen stilinde (Açıklama, Parametre, Dönüş Değeri) Türkçe yorum satırı ekle.
6. **Hata Yakalama:** Bağlantı kopmaları (WiFi/MQTT) ve sensör hataları için "LWT (Last Will)", "Watchdog" ve "Auto-Reconnect" mekanizmalarını zorunlu tut.