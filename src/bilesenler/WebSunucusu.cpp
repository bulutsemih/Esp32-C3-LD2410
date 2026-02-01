#include "WebSunucusu.h"
#include <WiFiManager.h>

// Yardımcılar
float getTemperature() { return temperatureRead(); }

// CSS Stili (Modern ve Sürgülü Slider Destekli)
String getStyle() {
  String s = "<style>";
  s += ":root { --primary: #2563eb; --success: #16a34a; --danger: #dc2626; "
       "--warning: #ca8a04; --bg: #f3f4f6; --card: #ffffff; --text: #1f2937; "
       "--border: #e5e7eb; --input-bg: #ffffff; --th-bg: #f9fafb; "
       "--table-border: #e2e8f0; }";
  s += "body.dark-mode { --bg: #111827; --card: #1f2937; --text: #f9fafb; "
       "--border: #374151; --input-bg: #374151; --th-bg: #374151; "
       "--table-border: #4b5563; }";
  s += "body { font-family: -apple-system, system-ui, sans-serif; "
       "background-color: var(--bg); "
       "color: var(--text); margin: 0; padding: 20px; line-height: 1.5; }";
  s +=
      ".container { max-width: 800px; margin: 0 auto; background: var(--card); "
      "padding: 20px; border-radius: 12px; box-shadow: 0 4px 6px "
      "rgba(0,0,0,0.1); }";
  s += ".section { margin-bottom: 25px; padding-top: 15px; border-top: 1px "
       "solid var(--border); }";
  s += "h2 { font-size: 1.25rem; margin-bottom: 1rem; color: var(--primary); }";
  s += ".form-group { margin-bottom: 1rem; }";
  s += "label { display: block; font-size: 0.875rem; font-weight: 500; "
       "margin-bottom: 0.5rem; }";
  s += "input[type='range'] { width: 100%; cursor: pointer; }";
  s += "input[type='text'], input[type='number'], input[type='password'] { "
       "width: 100%; padding: 10px; border: 1px solid var(--border); "
       "border-radius: 6px; box-sizing: border-box; background: "
       "var(--input-bg); color: var(--text); margin-bottom: 10px; }";
  s += ".val-display { font-weight: bold; color: var(--primary); float: right; "
       "}";
  s += "button { width: 100%; padding: 10px; background: var(--primary); "
       "color: white; "
       "border: none; border-radius: 6px; cursor: pointer; font-weight: 600; }";
  s += "button:hover { opacity: 0.9; }";
  s += ".nav-bar { display: flex; gap: 10px; margin-bottom: 20px; flex-wrap: "
       "wrap; }";
  s += ".nav-link { padding: 8px 12px; background: var(--border); "
       "border-radius: 6px; "
       "text-decoration: none; color: var(--text); font-size: 0.8rem; }";
  // Tablo Stilleri
  s += "table { width: 100%; border-collapse: collapse; margin-top: 10px; "
       "font-size: 0.9em; }";
  s += "th, td { padding: 10px; text-align: center; border: 1px solid "
       "var(--table-border); }";
  s += "th { background-color: var(--th-bg); font-weight: 600; color: "
       "var(--text); }";
  s += "tr:nth-child(even) { background-color: rgba(128,128,128,0.05); }";
  s += "</style>";
  return s;
}

// ... (Diğer fonksiyonlar aynı kalsın, sadece Nav barları güncellemek lazım ama
// önce yeni sayfaları ekleyelim)

void handleGatesApi() {
  JsonDocument doc;
  JsonArray gates = doc["gates"].to<JsonArray>();

  for (int i = 0; i < 9; i++) {
    JsonObject g = gates.add<JsonObject>();
    g["id"] = i;
    g["m_th"] = radar.motion_sensitivity[i];
    g["m_curr"] = radar.gate_moving_energy[i];
    g["s_th"] = radar.stationary_sensitivity[i];
    g["s_curr"] = radar.gate_stationary_energy[i];
  }

  String res;
  serializeJson(doc, res);
  server.send(200, "application/json", res);
}

void handleGatesPage() {
  String s = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta "
             "name='viewport' content='width=device-width, initial-scale=1.0'>";
  s += "<title>" + String(device_name) + " - Kapı Detayları</title>";
  s += getStyle();
  s += "</head><body><div class='container'>";

  // Nav
  s += "<div class='nav-bar'><a href='/' class='nav-link'>🏠 Ana Sayfa</a>";
  s += "<a href='/gates' class='nav-link' "
       "style='background:var(--primary);color:white'>📊 Kapı Detayları</a>";
  s += "<a href='/mqtt' class='nav-link'>🔗 MQTT</a>";
  s += "<a href='/advanced' class='nav-link'>⚙️ Gelişmiş</a></div>";

  s += "<h1>📊 Kapı Enerji Durumları</h1>";
  s += "<p>Aşağıdaki tablo, sensörün her bir mesafe dilimi (Gate) için "
       "algıladığı anlık enerji seviyelerini ve ayarlı eşik değerlerini "
       "gösterir. Kalibrasyonun etkisini burada görebilirsiniz.</p>";

  s += "<div class='section'>";
  s += "<table><thead><tr>";
  s += "<th>Gate</th><th>Mesafe</th><th>H. Eşik</th><th>H. Anlık</th><th>D. "
       "Eşik</th><th>D. Anlık</th>";
  s += "</tr></thead><tbody id='gateTable'><tr><td "
       "colspan='6'>Yükleniyor...</td></tr></tbody></table>";
  s += "</div>";

  s += "<script>";
  s += "function updateGates() {";
  s += "  fetch('/api/gates').then(r=>r.json()).then(d=>{";
  s += "    let h = '';";
  s += "    d.gates.forEach(g => {";
  s += "      let dist = (g.id * 0.75).toFixed(2) + ' - ' + ((g.id+1) * "
       "0.75).toFixed(2) + 'm';";
  s += "      h += "
       "`<tr><td>${g.id}</td><td>${dist}</td><td>${g.m_th}</"
       "td><td><b>${g.m_curr}</b></td><td>${g.s_th}</td><td><b>${g.s_curr}</"
       "b></td></tr>`;";
  s += "    });";
  s += "    document.getElementById('gateTable').innerHTML = h;";
  s += "  });";
  s += "}";
  s += "setInterval(updateGates, 1000); updateGates();";
  s += "</script>";

  s += "</div></body></html>";
  server.send(200, "text/html", s);
}

// ... (webSunucusuBaslat içine eklenecek)

// Ana Sayfa (Dashboard + Senaryo Ayarları)
void handleRoot() {
  String s = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta "
             "name='viewport' content='width=device-width, initial-scale=1.0'>";
  s += "<title>" + String(device_name) + " - Kontrol Paneli</title>";
  s += getStyle();
  s += "</head><body><div class='container'>";

  s += "<div class='nav-bar'>";
  s += "<a href='/' class='nav-link'>🏠 Ana Sayfa</a>";
  s += "<a href='/gates' class='nav-link'>📊 Kapı Detayları</a>";
  s += "<a href='/mqtt' class='nav-link'>🔗 MQTT Ayarları</a>";
  s += "<a href='/advanced' class='nav-link'>⚙️ Gelişmiş</a>";
  s += "</div>";

  s += "<h1>" + String(device_name) + "</h1>";

  // --- DİNAMİK SENARYO AYARLARI (AnKA-GLC Stili) ---
  s += "<div class='section'>";
  s += "<h2>🎯 Akıllı Senaryo Ayarları</h2>";
  s += "<form action='/save_scenario' method='POST'>";

  // Mesafe Eşiği Slider
  s += "<div class='form-group'>";
  s += "<label>Mesafe Eşiği (cm): <span class='val-display' id='distVal'>" +
       String(hareket_mesafe_esigi) + "</span></label>";
  s +=
      "<input type='range' name='h_dist' min='10' max='800' step='10' value='" +
      String(hareket_mesafe_esigi) +
      "' oninput='distVal.innerText=this.value'>";
  s += "</div>";

  // Enerji Eşiği Slider
  s += "<div class='form-group'>";
  s += "<label>Hareket Hassasiyeti (Enerji): <span class='val-display' "
       "id='engVal'>" +
       String(hareket_enerji_esigi) + "</span></label>";
  s += "<input type='range' name='h_eng' min='0' max='100' value='" +
       String(hareket_enerji_esigi) +
       "' oninput='engVal.innerText=this.value'>";
  s += "</div>";

  // --- DURAĞAN VARLIK AYARLARI ---
  s += "<br><h3>🛑 Durağan Varlık Ayarları</h3>";

  // Durağan Mesafe Eşiği Slider
  s += "<div class='form-group'>";
  s += "<label>Durağan Mesafe Eşiği (cm): <span class='val-display' "
       "id='dDistVal'>" +
       String(duragan_mesafe_esigi) + "</span></label>";
  s +=
      "<input type='range' name='d_dist' min='10' max='800' step='10' value='" +
      String(duragan_mesafe_esigi) +
      "' oninput='dDistVal.innerText=this.value'>";
  s += "</div>";

  // Durağan Enerji Eşiği Slider
  s += "<div class='form-group'>";
  s += "<label>Durağan Hassasiyet (Enerji): <span class='val-display' "
       "id='dEngVal'>" +
       String(duragan_enerji_esigi) + "</span></label>";
  s += "<input type='range' name='d_eng' min='0' max='100' value='" +
       String(duragan_enerji_esigi) +
       "' oninput='dEngVal.innerText=this.value'>";
  s += "</div>";

  // LED Alarm Toggle
  s += "<div class='form-group'>";
  s += "<label><input type='checkbox' name='l_alarm' " +
       String(led_alarm_modu ? "checked" : "") +
       "> Hareket Algılanınca Dahili LED'i Yak</label>";
  s += "</div>";

  s += "<button type='submit'>Senaryoyu Kaydet ve Uygula</button>";
  s += "</form></div>";

  // Sensör Verileri (JS ile güncellenir)
  s += "<div class='section'><h2>📊 Canlı Veriler</h2><div "
       "id='data'>Yükleniyor...</div></div>";

  s += "<script>";
  s += "setInterval(function(){ "
       "fetch('/api/sensors').then(r=>r.json()).then(d=>{";
  s += "let presHtml = d.presence === 'EVET' ? `<span "
       "style='color:var(--success);font-size:1.5em;font-weight:bold'>EVET</"
       "span>` : d.presence;";
  s += "document.getElementById('data').innerHTML = `<div "
       "style='display:grid;grid-template-columns:1fr "
       "1fr;gap:10px;margin-bottom:10px'>";
  s += "<div><h4 style='margin:0'>🏃 Hareketli</h4>Mesafe: <b>${d.mov_dist} "
       "cm</b><br>Enerji: <b>${d.mov_nrg}</b></div>";
  s += "<div><h4 style='margin:0'>🪑 Durağan</h4>Mesafe: <b>${d.stat_dist} "
       "cm</b><br>Enerji: <b>${d.stat_nrg}</b></div>";
  s += "</div>Varlık Durumu: ${presHtml}`;";
  s += "}); }, 1000);";
  s += "</script></div></body></html>";

  server.send(200, "text/html", s);
}

// Senaryo Kaydetme Handler'ı
void handleSaveScenario() {
  if (server.hasArg("h_dist")) {
    hareket_mesafe_esigi = server.arg("h_dist").toInt();
  }
  if (server.hasArg("h_eng")) {
    hareket_enerji_esigi = server.arg("h_eng").toInt();
  }
  if (server.hasArg("d_dist")) {
    duragan_mesafe_esigi = server.arg("d_dist").toInt();
  }
  if (server.hasArg("d_eng")) {
    duragan_enerji_esigi = server.arg("d_eng").toInt();
  }

  led_alarm_modu = server.hasArg("l_alarm");

  // Ayarları Preferences (Kalıcı Hafıza) içine kaydet
  preferences.begin("radar-app", false);
  preferences.putInt("h_dist", hareket_mesafe_esigi);
  preferences.putInt("h_eng", hareket_enerji_esigi);
  preferences.putInt("d_dist", duragan_mesafe_esigi);
  preferences.putInt("d_eng", duragan_enerji_esigi);
  preferences.putBool("l_alarm", led_alarm_modu);
  preferences.end();

  // Ana sayfaya geri dön
  server.sendHeader("Location", "/");
  server.send(303);
}

// API: Sensör verilerini JSON olarak döner
void handleJson() {
  JsonDocument doc;
  doc["presence"] = insan_varligi ? "EVET" : "HAYIR";
  doc["mov_dist"] = radar.movingTargetDistance();
  doc["mov_nrg"] = radar.movingTargetEnergy();
  doc["stat_dist"] = radar.stationaryTargetDistance();
  doc["stat_nrg"] = radar.stationaryTargetEnergy();
  doc["temp"] = getTemperature();

  String res;
  serializeJson(doc, res);
  server.send(200, "application/json", res);
}

// MQTT Ayarları Sayfası
void handleMqtt() {
  String s = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta "
             "name='viewport' content='width=device-width, initial-scale=1.0'>";
  s += "<title>" + String(device_name) + " - MQTT Ayarları</title>";
  s += getStyle();
  s += "</head><body><div class='container'>";

  // Nav
  s += "<div class='nav-bar'><a href='/' class='nav-link'>🏠 Ana Sayfa</a><a "
       "href='/mqtt' class='nav-link' "
       "style='background:var(--primary);color:white'>🔗 MQTT Ayarları</a><a "
       "href='/advanced' class='nav-link'>⚙️ Gelişmiş</a></div>";

  s += "<h1>MQTT Yapılandırması</h1>";
  s += "<div class='section'><form action='/save_mqtt' method='POST'>";

  s += "<div class='form-group'><label>MQTT Sunucu IP:</label>";
  s += "<input type='text' name='server' value='" + String(mqtt_server) +
       "'></div>";

  s += "<div class='form-group'><label>MQTT Port:</label>";
  s += "<input type='number' name='port' value='" + String(mqtt_port) +
       "'></div>";

  s += "<div class='form-group'><label>Kullanıcı Adı (Opsiyonel):</label>";
  s +=
      "<input type='text' name='user' value='" + String(mqtt_user) + "'></div>";

  s += "<div class='form-group'><label>Şifre (Opsiyonel):</label>";
  s += "<input type='password' name='pass' value='" + String(mqtt_pass) +
       "'></div>";

  s += "<button type='submit'>Ayarları Kaydet</button>";
  s += "</form></div></div></body></html>";
  server.send(200, "text/html", s);
}

// MQTT Kaydet
void handleSaveMqtt() {
  if (server.hasArg("server"))
    server.arg("server").toCharArray(mqtt_server, 40);
  if (server.hasArg("port"))
    mqtt_port = server.arg("port").toInt();
  if (server.hasArg("user"))
    server.arg("user").toCharArray(mqtt_user, 32);
  if (server.hasArg("pass"))
    server.arg("pass").toCharArray(mqtt_pass, 32);

  preferences.begin("radar-app", false);
  preferences.putString("mqtt_server", mqtt_server);
  preferences.putInt("mqtt_port", mqtt_port);
  preferences.putString("mqtt_user", mqtt_user);
  preferences.putString("mqtt_pass", mqtt_pass);
  preferences.end();

  if (strlen(mqtt_server) > 0) {
    mqttClient.setServer(mqtt_server, mqtt_port);
  }

  server.sendHeader("Location", "/mqtt");
  server.send(303);
}

// Gelişmiş Ayarlar Sayfası
void handleAdvanced() {
  String s = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta "
             "name='viewport' content='width=device-width, initial-scale=1.0'>";
  s += "<title>" + String(device_name) + " - Gelişmiş Ayarlar</title>";
  s += getStyle();
  s += "</head><body><div class='container'>";

  // Nav
  s += "<div class='nav-bar'><a href='/' class='nav-link'>🏠 Ana Sayfa</a><a "
       "href='/mqtt' class='nav-link'>🔗 MQTT Ayarları</a><a href='/advanced' "
       "class='nav-link' style='background:var(--primary);color:white'>⚙️ "
       "Gelişmiş</a></div>";

  s += "<h1>Gelişmiş Ayarlar</h1>";

  // Cihaz Adı
  s += "<div class='section'><h2>✏️ Cihaz Kimliği</h2>";
  s += "<form action='/save_advanced' method='POST'>";
  s += "<div class='form-group'><label>Cihaz Adı:</label>";
  s += "<input type='text' name='devname' value='" + String(device_name) +
       "'></div>";
  s += "<button type='submit'>İsim Değiştir</button></form></div>";

  // Boş Oda Kalibrasyonu (Sihirbaz)
  s += "<div class='section'><h2>🛠️ Boş Oda Kalibrasyonu</h2>";
  s += "<p style='font-size:0.9em;color:var(--text)'>Bu işlem, odadaki duvar, "
       "koltuk gibi eşyaları tarayarak yanlış alarmları önler. Başlattıktan "
       "sonra odadan çıkmanız gerekir.</p>";
  s += "<form action='/calibrate' method='GET'><button type='submit' "
       "style='background:var(--success)'>Sihirbazı Başlat</button></form>";
  s += "</div>";

  // Sistem Eylemleri
  s += "<div class='section'><h2>⚡ Sistem Eylemleri</h2>";
  s += "<div style='display:flex;gap:10px'>";
  s += "<form action='/restart' method='POST' style='flex:1'><button "
       "type='submit' style='background:var(--warning)'>Cihazı Yeniden "
       "Başlat</button></form>";
  s += "<form action='/reset_wifi' method='POST' style='flex:1'><button "
       "type='submit' style='background:var(--danger)' onclick=\"return "
       "confirm('WiFi ayarları silinecek, emin misiniz?')\">WiFi "
       "Sıfırla</button></form>";
  s += "</div></div>";

  s += "</div></body></html>";
  server.send(200, "text/html", s);
}

// Gelişmiş Kaydet
void handleSaveAdvanced() {
  if (server.hasArg("devname")) {
    server.arg("devname").toCharArray(device_name, 32);
    preferences.begin("radar-app", false);
    preferences.putString("device_name", device_name);
    preferences.end();
  }
  server.sendHeader("Location", "/advanced");
  server.send(303);
}

void handleRestart() {
  String s = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta "
             "http-equiv='refresh' content='10;url=/'><meta name='viewport' "
             "content='width=device-width, initial-scale=1.0'>";
  s += "<title>Yeniden Başlatılıyor</title>";
  s += getStyle();
  s += "</head><body><div class='container' style='text-align:center'>";
  s += "<h1>🔄 Yeniden Başlatılıyor...</h1>";
  s += "<p>Lütfen bekleyin, cihaz yeniden başlatılıyor. 10 saniye içinde ana "
       "sayfaya yönlendirileceksiniz.</p>";
  s += "<div style='margin:20px 0;font-size:30px'>⏳ <span "
       "id='cnt'>10</span></div>";
  s += "<script>let c=10;setInterval(()=>{c--;document.getElementById('cnt')."
       "innerText=c;if(c<=0)window.location.href='/';},1000);</script>";
  s += "</div></body></html>";
  server.send(200, "text/html", s);

  delay(1000); // Sayfanın gitmesi için kısa bir bekleme
  ESP.restart();
}

void handleCalibrationPage() {
  String s = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta "
             "name='viewport' content='width=device-width, initial-scale=1.0'>";
  s += "<title>" + String(device_name) + " - Boş Oda Kalibrasyonu</title>";
  s += getStyle();
  s += "</head><body><div class='container' style='text-align:center'>";
  s += "<h1>🛠️ Boş Oda Kalibrasyonu</h1>";
  s += "<div id='cnt' style='font-size:20px; margin:20px 0;'>10 Saniye içinde "
       "odadan çıkın!</div>";
  s += "<div id='status' style='font-size:18px; color:var(--primary); "
       "font-weight:bold'></div>";
  s += "<button onclick=\"window.location.href='/advanced'\" "
       "style='margin-top:20px;background:var(--border);color:var(--text)'>"
       "İptal / Geri Dön</button>";

  s += "<script>";
  s += "let c=10;";
  s += "const statusDiv = document.getElementById('status');";

  // Geri Sayım ve Başlatma
  s += "let timer = setInterval(()=>{";
  s += "  c--; document.getElementById('cnt').innerText = c + ' saniye içinde "
       "odadan çıkın!';";
  s += "  if(c<=0) {";
  s += "    clearInterval(timer);";
  s += "    document.getElementById('cnt').innerText = 'Kalibrasyon "
       "Başlıyor...';";
  s += "    fetch('/action_calibrate', {method:'POST'}).then(()=>{";
  s += "       checkStatus();";
  s += "    });";
  s += "  }";
  s += "}, 1000);";

  // Durum Kontrolü
  s += "function checkStatus(){";
  s += "  setInterval(()=>{";
  s += "    fetch('/status_calibrate').then(r=>r.text()).then(st=>{";
  s += "      if(st==='CALIBRATING') statusDiv.innerText = 'Veri Toplanıyor... "
       "Lütfen bekleyin.';";
  s += "      else if(st==='DONE') {";
  s += "          statusDiv.innerText = '✅ Kalibrasyon Tamamlandı!';";
  s += "          statusDiv.style.color = 'var(--success)';";
  s += "          setTimeout(()=>{window.location.href='/advanced'}, 3000);";
  s += "      }";
  s += "    });";
  s += "  }, 1000);";
  s += "}";
  s += "</script></div></body></html>";
  server.send(200, "text/html", s);
}

void handleActionCalibrate() {
  kalibrasyonBaslat();
  server.send(200, "text/plain", "OK");
}

void handleStatusCalibrate() {
  server.send(200, "text/plain", kalibrasyonDurumu());
}

// Web Sunucusu Başlatma ve Rotalar
void webSunucusuBaslat() {
  server.on("/", handleRoot);
  server.on("/api/sensors", handleJson);
  server.on("/save_scenario", HTTP_POST, handleSaveScenario);

  // Yeni Rotalar
  server.on("/mqtt", handleMqtt);
  server.on("/save_mqtt", HTTP_POST, handleSaveMqtt);
  server.on("/advanced", handleAdvanced);
  server.on("/save_advanced", HTTP_POST, handleSaveAdvanced);
  server.on("/restart", HTTP_POST, handleRestart);

  // Kalibrasyon Rotaları
  server.on("/calibrate", handleCalibrationPage);
  server.on("/action_calibrate", HTTP_POST, handleActionCalibrate);
  server.on("/status_calibrate", handleStatusCalibrate);

  // Kapı Detayları
  server.on("/gates", handleGatesPage);
  server.on("/api/gates", handleGatesApi);

  // WiFi Reset (Hem GET hem POST desteklesin)
  auto resetWifiHandler = []() {
    server.send(200, "text/plain", "WiFi Ayarları Sıfırlanıyor...");
    delay(1000);
    WiFiManager wm;
    wm.resetSettings();
    ESP.restart();
  };
  server.on("/reset_wifi", resetWifiHandler);
  server.on("/reset_wifi", HTTP_POST, resetWifiHandler);

  server.begin();
  Serial.println("✓ Web Sunucusu Başlatıldı.");
}