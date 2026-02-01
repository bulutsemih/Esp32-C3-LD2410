#ifndef RADARSENSORU_H
#define RADARSENSORU_H

#include "../Degiskenler.h"

void radarBaslat();
void radarDongu();
void forceEngineeringMode();

// Kalibrasyon Fonksiyonları
void kalibrasyonBaslat();
String kalibrasyonDurumu();
extern bool kalibrasyon_aktif;

#endif
