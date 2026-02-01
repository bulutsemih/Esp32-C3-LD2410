#include "AgBaglantisi.h"

void publishHomeAssistantDiscovery() {
  if (!mqttClient.connected())
    return;

  String mac = WiFi.macAddress();
  mac.replace(":", "");

  String safeName = String(device_name);
  safeName.trim();
  safeName.replace(" ", "_");
  safeName.toLowerCase();
  String uniqueId = "ld2410_" + safeName;
  String stateTopic =
      "ld2410/" + safeName +
      "/durum"; // 'state' yerine 'durum' (Türkçe topik isteği üzerine, ama
                // genelde state standarddır. Kullanıcı JSON keyleri istediği
                // için state kalabilir ama topic yapısını da türkçeleştirelim:
                // state -> durum)
  // Düzeltme: Home Assistant stat_t değişirse karışıklık olabilir ama tutarlı
  // yaparsak sorun yok. Topic root'u main loop'ta 'durum' olarak
  // güncelleyeceğim.

  // Cihaz Bilgisi
  JsonObject device;
  JsonDocument docDev;
  device = docDev.to<JsonObject>();
  device["ids"] = uniqueId;
  device["name"] = device_name;
  device["mdl"] = "LD2410C";
  device["mf"] = "Hi-Link";
  device["sw"] = "v1.1 (TR)";

  // 1. Varlık
  {
    JsonDocument doc;
    doc["name"] = "Varlık";
    doc["uniq_id"] = uniqueId + "_varlik";
    doc["stat_t"] = stateTopic;
    doc["val_tpl"] = "{{ value_json.varlik }}";
    doc["dev_cla"] = "occupancy";
    doc["dev"] = device;

    String payload;
    serializeJson(doc, payload);
    mqttClient.publish(
        ("homeassistant/binary_sensor/" + uniqueId + "/varlik/config").c_str(),
        payload.c_str(), true);
  }

  // 2. Hareket Mesafesi
  {
    JsonDocument doc;
    doc["name"] = "Hareket Mesafesi";
    doc["uniq_id"] = uniqueId + "_hareket_mesafesi";
    doc["stat_t"] = stateTopic;
    doc["val_tpl"] = "{{ value_json.hareket_mesafesi }}";
    doc["unit_of_meas"] = "cm";
    doc["dev_cla"] = "distance";
    doc["dev"] = device;

    String payload;
    serializeJson(doc, payload);
    mqttClient.publish(
        ("homeassistant/sensor/" + uniqueId + "/hareket_mesafesi/config")
            .c_str(),
        payload.c_str(), true);
  }

  // 3. Hareket Enerjisi
  {
    JsonDocument doc;
    doc["name"] = "Hareket Enerjisi";
    doc["uniq_id"] = uniqueId + "_hareket_enerjisi";
    doc["stat_t"] = stateTopic;
    doc["val_tpl"] = "{{ value_json.hareket_enerjisi }}";
    doc["unit_of_meas"] = "%";
    doc["dev"] = device;

    String payload;
    serializeJson(doc, payload);
    mqttClient.publish(
        ("homeassistant/sensor/" + uniqueId + "/hareket_enerjisi/config")
            .c_str(),
        payload.c_str(), true);
  }

  // 4. Duran Mesafe
  {
    JsonDocument doc;
    doc["name"] = "Duran Mesafe";
    doc["uniq_id"] = uniqueId + "_duran_mesafe";
    doc["stat_t"] = stateTopic;
    doc["val_tpl"] = "{{ value_json.duran_mesafe }}";
    doc["unit_of_meas"] = "cm";
    doc["dev_cla"] = "distance";
    doc["dev"] = device;

    String payload;
    serializeJson(doc, payload);
    mqttClient.publish(
        ("homeassistant/sensor/" + uniqueId + "/duran_mesafe/config").c_str(),
        payload.c_str(), true);
  }

  // 5. Duran Enerji (Eksikti, eklendi)
  {
    JsonDocument doc;
    doc["name"] = "Duran Enerji";
    doc["uniq_id"] = uniqueId + "_duran_enerji";
    doc["stat_t"] = stateTopic;
    doc["val_tpl"] = "{{ value_json.duran_enerji }}";
    doc["unit_of_meas"] = "%";
    doc["dev"] = device;

    String payload;
    serializeJson(doc, payload);
    mqttClient.publish(
        ("homeassistant/sensor/" + uniqueId + "/duran_enerji/config").c_str(),
        payload.c_str(), true);
  }

  // 6. Sıcaklık
  {
    JsonDocument doc;
    doc["name"] = "Sıcaklık";
    doc["uniq_id"] = uniqueId + "_sicaklik";
    doc["stat_t"] = stateTopic;
    doc["val_tpl"] = "{{ value_json.sicaklik }}";
    doc["unit_of_meas"] = "°C";
    doc["dev_cla"] = "temperature";
    doc["dev"] = device;

    String payload;
    serializeJson(doc, payload);
    mqttClient.publish(
        ("homeassistant/sensor/" + uniqueId + "/sicaklik/config").c_str(),
        payload.c_str(), true);
  }

  // 7. Sinyal Gücü
  {
    JsonDocument doc;
    doc["name"] = "WiFi Sinyal";
    doc["uniq_id"] = uniqueId + "_sinyal_gucu";
    doc["stat_t"] = stateTopic;
    doc["val_tpl"] = "{{ value_json.sinyal_gucu }}";
    doc["unit_of_meas"] = "dBm";
    doc["dev_cla"] = "signal_strength";
    doc["dev"] = device;

    String payload;
    serializeJson(doc, payload);
    mqttClient.publish(
        ("homeassistant/sensor/" + uniqueId + "/sinyal_gucu/config").c_str(),
        payload.c_str(), true);
  }

  // 8. Çalışma Süresi
  {
    JsonDocument doc;
    doc["name"] = "Çalışma Süresi";
    doc["uniq_id"] = uniqueId + "_calisma_suresi";
    doc["stat_t"] = stateTopic;
    doc["val_tpl"] = "{{ value_json.calisma_suresi }}";
    doc["unit_of_meas"] = "s";
    doc["dev_cla"] = "duration";
    doc["dev"] = device;

    String payload;
    serializeJson(doc, payload);
    mqttClient.publish(
        ("homeassistant/sensor/" + uniqueId + "/calisma_suresi/config").c_str(),
        payload.c_str(), true);
  }
}

void mqttDongu() {
  if (strlen(mqtt_server) == 0)
    return;

  if (!mqttClient.connected()) {
    if (millis() - lastMqttRetry > 5000) {
      lastMqttRetry = millis();

      String clientId = "LD2410-" + String(random(0xffff), HEX);

      if (mqttClient.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
        if (mqtt_platform == 0) {
          publishHomeAssistantDiscovery();
        }
      }
    }
  } else {
    mqttClient.loop();

    if (millis() - lastMqttPublish > 500) {
      lastMqttPublish = millis();

      String safeName = String(device_name);
      safeName.trim();
      safeName.replace(" ", "_");
      safeName.toLowerCase();
      String topicRoot = "ld2410/" + safeName + "/";

      // Sistem verilerini oku
      float temp = temperatureRead();
      int rssi = WiFi.RSSI();
      unsigned long uptime = millis() / 1000;

      if (radar.isConnected()) {
        if (mqtt_platform == 0 || mqtt_platform == 1) {
          JsonDocument doc;
          // Türkçe Anahtarlar
          // DÜZELTME: Ham veri yerine islenmis veriyi kullan
          doc["varlik"] = insan_varligi ? "ON" : "OFF";
          doc["hareket_mesafesi"] = radar.movingTargetDistance();
          doc["hareket_enerjisi"] = radar.movingTargetEnergy();
          doc["duran_mesafe"] = radar.stationaryTargetDistance();
          doc["duran_enerji"] = radar.stationaryTargetEnergy();

          // Sistem verileri
          doc["sicaklik"] = temp;
          doc["sinyal_gucu"] = rssi;
          doc["calisma_suresi"] = uptime;

          String payload;
          serializeJson(doc, payload);
          // Topic: .../durum (state yerine)
          mqttClient.publish((topicRoot + "durum").c_str(), payload.c_str());
        }

        if (mqtt_platform == 2) {
          // Ayrı Konular (Türkçe Topicler)
          // DÜZELTME: Ham veri yerine islenmis veriyi kullan
          mqttClient.publish((topicRoot + "varlik").c_str(),
                             insan_varligi ? "ON" : "OFF");
          mqttClient.publish((topicRoot + "hareket_mesafesi").c_str(),
                             String(radar.movingTargetDistance()).c_str());
          mqttClient.publish((topicRoot + "hareket_enerjisi").c_str(),
                             String(radar.movingTargetEnergy()).c_str());
          mqttClient.publish((topicRoot + "duran_mesafe").c_str(),
                             String(radar.stationaryTargetDistance()).c_str());
          mqttClient.publish((topicRoot + "duran_enerji").c_str(),
                             String(radar.stationaryTargetEnergy()).c_str());

          mqttClient.publish((topicRoot + "sicaklik").c_str(),
                             String(temp).c_str());
          mqttClient.publish((topicRoot + "sinyal_gucu").c_str(),
                             String(rssi).c_str());
          mqttClient.publish((topicRoot + "calisma_suresi").c_str(),
                             String(uptime).c_str());
        }
      }
    }
  }
}
