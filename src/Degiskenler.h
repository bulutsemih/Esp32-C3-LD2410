#ifndef DEGISKENLER_H
#define DEGISKENLER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <WiFi.h>
#include <ld2410.h>

// Degiskenler.h içine eklenecekler
extern int hareket_mesafe_esigi;
extern int hareket_enerji_esigi;
extern bool led_alarm_modu;

// Pin Tanımları
extern const int RX_PIN;  // GPIO 7
extern const int TX_PIN;  // GPIO 6
extern const int LED_PIN; // GPIO 8

// Nesneler
extern ld2410 radar;
extern WebServer server;
extern Preferences preferences;
extern WiFiClient espClient;
extern PubSubClient mqttClient;
extern HardwareSerial RadarSerial;

// Değişkenler
extern uint8_t ld2410_last_frame_type;
extern char mqtt_server[40];
extern int mqtt_port;
extern char mqtt_user[32];
extern char mqtt_pass[32];
extern char device_name[32];
extern int mqtt_platform;
extern String nodeId;

extern unsigned long lastMqttRetry;
extern unsigned long lastMqttPublish;
extern unsigned long lastLogTime;
extern unsigned long lastEngineeringCheck;
extern unsigned long lastRadarFixAttempt;

extern bool shouldSaveConfig;

extern int hareket_mesafe_esigi;
extern int hareket_enerji_esigi;
extern int duragan_mesafe_esigi;
extern int duragan_enerji_esigi;
extern bool led_alarm_modu;
extern bool insan_varligi;
extern unsigned long son_varlik_zamani;

// Callback
void saveConfigCallback();

#endif
