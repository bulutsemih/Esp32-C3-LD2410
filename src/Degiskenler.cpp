#include "Degiskenler.h"

// Pin Tanımları
const int RX_PIN = 6;
const int TX_PIN = 7;
const int LED_PIN = 8;

// Nesneler
ld2410 radar;
WebServer server(80);
Preferences preferences;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
HardwareSerial RadarSerial(1);

// Değişkenler
// ld2410_last_frame_type kütüphanede tanımlı olabilir, ancak burada extern
// olarak belirtmiştik. Eğer kütüphane içinde global ise burada tanımlamayız.
// Ancak main.cpp'de "extern uint8_t ld2410_last_frame_type;" vardı, yani başka
// bir yerde tanımlı. Büyük ihtimalle ld2410.cpp içinde tanımlı. O yüzden burada
// tekrar tanımlamıyoruz.

// Degiskenler.cpp içine eklenecekler
int hareket_mesafe_esigi = 600; // Varsayılan 1.5 metre
int hareket_enerji_esigi = 50;  // Varsayılan %50 enerji
int duragan_mesafe_esigi = 600; // Varsayılan 6.0 metre (Durağan için)
int duragan_enerji_esigi = 50;  // Varsayılan %50 enerji (Durağan için)
bool led_alarm_modu = true;     // Varsayılan LED açık
bool insan_varligi = false;
unsigned long son_varlik_zamani = 0;

char mqtt_server[40] = "";
int mqtt_port = 1883;
char mqtt_user[32] = "";
char mqtt_pass[32] = "";
char device_name[32] = "BULUT Radar";
int mqtt_platform = 0;
String nodeId = "ld2410_radar";

unsigned long lastMqttRetry = 0;
unsigned long lastMqttPublish = 0;
unsigned long lastLogTime = 0;
unsigned long lastEngineeringCheck = 0;
unsigned long lastRadarFixAttempt = 0;

bool shouldSaveConfig = false;

void saveConfigCallback() { shouldSaveConfig = true; }
