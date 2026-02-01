# LD2410 Kütüphanesi Düzeltme Notu (v0.1.4)

**Sorun:**
LD2410 kütüphanesinin v0.1.4 sürümünde, ayar komutları (örneğin `setMaxValues` veya `setGateSensitivityThreshold`) gönderildiğinde "Ayarlar Kaydedilemedi" hatası alınıyor veya işlem zaman aşımına uğruyor.

**Sebep:**
Kütüphane geliştiricisi veri okuma işlemini "döngüsel tampon" (circular buffer) yapısına geçirmiş. Ancak, komut gönderdikten sonra cevabın beklendiği `while` döngüleri içinde, UART'tan gelen veriyi bu tampona aktaran kod bloğu unutulmuş. Bu yüzden ESP32 cevabı bekliyor ama cevap tampona girmediği için okuyamıyor.

**Çözüm:**
`src/ld2410.cpp` dosyasındaki tüm komut bekleme döngülerine (yaklaşık 10-12 farklı fonksiyon) aşağıdaki kod bloğu eklendi:

```cpp
while(radar_uart_->available())
{
    add_to_buffer(radar_uart_->read());
}
```

**Düzeltilen Fonksiyonlar:**
- `enter_configuration_mode_`
- `leave_configuration_mode_`
- `requestStartEngineeringMode`
- `requestEndEngineeringMode`
- `requestCurrentConfiguration`
- `requestFirmwareVersion`
- `requestRestart`
- `requestFactoryReset`
- `setMaxValues`
- `setGateSensitivityThreshold`

*Not: Bu düzeltme yapılmazsa, PlatformIO kütüphaneyi güncellediğinde veya proje temizlendiğinde (clean) ayarlar tekrar bozulabilir.*