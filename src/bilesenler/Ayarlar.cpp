#include "Ayarlar.h"

void ayarlariYukle() {
  // Ayarları Yükle
  preferences.begin("radar-app", false);
  String s_name = preferences.getString("device_name", "BULUT Radar");
  s_name.toCharArray(device_name, 32);

  // ayarlariYukle() fonksiyonu içine eklenecekler:
  hareket_mesafe_esigi = preferences.getInt("h_dist", 150);
  hareket_enerji_esigi = preferences.getInt("h_eng", 50);
  duragan_mesafe_esigi = preferences.getInt("d_dist", 600);
  duragan_enerji_esigi = preferences.getInt("d_eng", 50);
  led_alarm_modu = preferences.getBool("l_alarm", true);

  mqtt_platform = preferences.getInt("mqtt_platform", 0);
  String s_server = preferences.getString("mqtt_server", "");
  s_server.toCharArray(mqtt_server, 40);
  mqtt_port = preferences.getInt("mqtt_port", 1883);
  String s_user = preferences.getString("mqtt_user", "");
  s_user.toCharArray(mqtt_user, 32);
  String s_pass = preferences.getString("mqtt_pass", "");
  s_pass.toCharArray(mqtt_pass, 32);
  preferences.end();

  // --- WiFiManager ---
  WiFiManager wm;
  wm.setSaveConfigCallback(saveConfigCallback);

  WiFiManagerParameter custom_device_name("devname", "Cihaz Adi (Orn: Salon)",
                                          device_name, 32);
  WiFiManagerParameter custom_mqtt_server("server", "MQTT Sunucu IP",
                                          mqtt_server, 40);

  char c_mqtt_port[6];
  sprintf(c_mqtt_port, "%d", mqtt_port);
  WiFiManagerParameter custom_mqtt_port("port", "MQTT Port", c_mqtt_port, 6);

  WiFiManagerParameter custom_mqtt_user("user", "MQTT Kullanici (Opsiyonel)",
                                        mqtt_user, 32);
  WiFiManagerParameter custom_mqtt_pass("pass", "MQTT Sifre (Opsiyonel)",
                                        mqtt_pass, 32, "type='password'");

  wm.addParameter(&custom_device_name);
  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_port);
  wm.addParameter(&custom_mqtt_user);
  wm.addParameter(&custom_mqtt_pass);

  wm.setAPCallback(
      [](WiFiManager *myWiFiManager) { digitalWrite(LED_PIN, HIGH); });

  WiFi.setHostname(device_name);

  bool res = wm.autoConnect("BULUT-Radar-Setup");

  if (!res) {
    // Bağlantı başarısızsa ne yapacağına karar verilebilir
  }

  if (shouldSaveConfig) {
    strcpy(device_name, custom_device_name.getValue());
    strcpy(mqtt_server, custom_mqtt_server.getValue());
    mqtt_port = atoi(custom_mqtt_port.getValue());
    strcpy(mqtt_user, custom_mqtt_user.getValue());
    strcpy(mqtt_pass, custom_mqtt_pass.getValue());

    preferences.begin("radar-app", false);
    preferences.putString("device_name", device_name);
    preferences.putString("mqtt_server", mqtt_server);
    preferences.putInt("mqtt_port", mqtt_port);
    preferences.putString("mqtt_user", mqtt_user);
    preferences.putString("mqtt_pass", mqtt_pass);
    preferences.end();
  }

  // Isinma Sorunu İcin: WiFi Uyku Modunu Aktif Et (true)
  // Bu islem web arayuzunde milisaniyelik gecikme yapabilir ama isiyi cok
  // dusurur.
  WiFi.setSleep(true);
  digitalWrite(LED_PIN, LOW);

  if (strlen(mqtt_server) > 0) {
    mqttClient.setServer(mqtt_server, mqtt_port);
  }
}
