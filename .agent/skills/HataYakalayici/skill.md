Ad: HataYakalayici

Açıklama: Kodun çalışma süresindeki (runtime) hataları yönetir.

Talimat:

    1. WiFi ve MQTT bağlantıları için reconnect() döngüleri ekle.

    2. Sensör okuma hatalarında (Timeout, Checksum error) sistemi kilitlenme yerine "Güvenli Mod"a al.

    3. Kritik hataları seri port üzerinden [HATA], bilgilendirmeleri [BILGI] etiketiyle logla.

    4. Gerektiğinde ESP.restart() gibi otomatik kurtarma mekanizmalarını plana dahil et.