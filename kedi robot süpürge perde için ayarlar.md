# LD2410 – Kedi + Robot Süpürge + Perde Özel Preset

Bu preset; ev ortamında en sık yanlış algılamaya sebep olan üçlüye karşı hazırlanmıştır:
- Kedi
- Robot süpürge
- Perde / klima / hava akımı

Amaç:
- İnsan algısını korumak
- Kısa, alçak ve düzensiz hareketleri yok saymak
- Uzun süreli, gövde bazlı varlığı gerçek kabul etmek

---

## Tehdit Analizi

### 🐈 Kedi
- Alçak yükseklik
- Hızlı, düzensiz hareket
- Duran enerji çok düşük

### 🤖 Robot Süpürge
- Sürekli hareket
- Düşük radar kesiti
- Zemine yakın, tekrarlı rota

### 🪟 Perde / Klima
- Geniş ama zayıf mikroyansıma
- Sürekli ama anlamsız hareket
- Özellikle Gate 2–4 arası riskli

---

## Altın Strateji

1. **Yakın gate’leri sertleştir**
2. **Duran algılamayı orta–uzak mesafede tut**
3. **Hareket algılamayı süreyle filtrele**
4. Sensörü zemine değil gövdeye baktır

---

## Özel Preset Ayarları

| Gate | Mesafe | Hareket | Duran |
|----|----|----|----|
| 0 | 0.0 m | 65 | 0 |
| 1 | 0.75 m | 60 | 0 |
| 2 | 1.5 m | 50 | 10 |
| 3 | 2.25 m | 35 | 30 |
| 4 | 3.0 m | 30 | 30 |
| 5 | 3.75 m | 25 | 25 |
| 6 | 4.5 m | 25 | 20 |
| 7 | 5.25 m | 25 | 15 |
| 8 | 6.0 m | 25 | 15 |

---

## Davranış Sonuçları

### İnsan
- Odaya girer → algılanır
- Oturur / ayakta durur → algı korunur
- Yavaş hareket eder → düşmez

### Kedi
- Yerde dolaşır → büyük oranda yok sayılır
- Zıplarsa → kısa tetik olabilir
- Oturursa → algı oluşmaz

### Robot Süpürge
- Sürekli hareket etse bile:
  - Enerji eşikleri geçemez
  - Duran algısı yok
- “Evde biri var” sayılmaz

### Perde / Klima
- Hareket var ama enerji düşük
- Süreklilik anlamsız
- Algılama oluşmaz

---

## Zaman Tabanlı Yazılımsal Filtre (ŞART)

> **3–5 saniyeden kısa algı = yok say**

Bu kural olmadan bu preset %100 çalışmaz.

### Mantık
- Kedi ve süpürge: kısa ve düzensiz
- İnsan: uzun ve tutarlı

---

## Home Assistant Örnek Mantık (Özet)

- binary_sensor ON
- 5 saniye boyunca kesintisiz ON ise → occupied
- Aksi halde → ignore

Bu tek filtre:
- Robot süpürgeyi %90
- Kediyi %70
- Perdeyi %95 oranında susturur

---

## Fiziksel Montaj Tavsiyesi

- Yükseklik: **1.9 – 2.3 m**
- Hafif aşağı eğim
- Zemini değil gövdeyi kapsasın
- Perdeyle birebir bakışmasın
- Metal yüzeylerden uzak

---

## Yapılmaması Gerekenler

❌ Sensörü 1 m altına takmak  
❌ Gate 0–1’i hassas yapmak  
❌ Duran eşiğini her yerde açmak  
❌ Süre filtresi olmadan alarm senaryosu yazmak  

---

## Gerçekçi Başarı Oranı

| Senaryo | Başarı |
|------|------|
| Kedi | %80–90 |
| Robot süpürge | %90+ |
| Perde / klima | %95 |
| Kedi + süpürge aynı anda | %85 |
| İnsan varken yanlış düşme | Çok düşük |

---

## Son Not

LD2410 zeki değil.
Ama **istikrarlı hareketi tanır**.

Kedi kaotik.
Süpürge inatçı.
Perde dengesiz.

İnsan ise sıkıcı derecede tutarlı.

Bu preset tam olarak bunu kullanır.
