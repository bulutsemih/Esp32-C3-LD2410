# LD2410 İnce Ayar Rehberi

Bu doküman LD2410 radar varlık sensörünün **Gate (mesafe) bazlı hassasiyet ayarlarını** ve gerçek hayatta nasıl davrandıklarını açıklar.

---

## Temel Kavramlar

- **Gate**: Mesafe dilimi (yaklaşık 0.75 m)
- **Anlık Enerji**: Sensörün ölçtüğü radar yansıması
- **Eşik Değeri**: Algılama için gereken minimum enerji
- **Kural**:  
  Anlık Enerji > Eşik Değeri → Algılama var

Hareketli ve duran hedefler **ayrı ayrı** değerlendirilir.

---

## Gate Mantığı

| Gate | Mesafe (yaklaşık) |
|----|----|
| 0 | 0.0 m |
| 1 | 0.75 m |
| 2 | 1.5 m |
| 3 | 2.25 m |
| 4 | 3.0 m |
| 5 | 3.75 m |
| 6 | 4.5 m |
| 7 | 5.25 m |
| 8 | 6.0 m |

Yakın gate’ler yüksek eşik ister, uzak gate’ler daha hassas olur.

---

## Sensör Davranışı Örnekleri

### Gate 1 (0.75 m)
- Hareket eşiği yüksekse: küçük hareketler algılanmaz
- Duran = 0 ise: sabit duran insan yok sayılır

### Gate 3 (2.25 m)
- Hareket düşük eşik: yavaş yürüyen algılanır
- Duran yüksek eşik: uzun süre sabit kalırsa düşer

### Gate 6–8 (4.5–6 m)
- Hareket algılama öncelikli
- Duran algılama zayıf tutulur
- Amaç: yanlış pozitifleri engellemek

---

## Preset Ayarlar

### 1. Oda İçi (Salon / Yatak Odası)

Amaç: İnsan varken algı kalsın, perde tetiklemesin.

| Gate | Hareket | Duran |
|----|----|----|
| 0 | 50 | 0 |
| 1 | 45 | 0 |
| 2 | 35 | 35 |
| 3 | 30 | 35 |
| 4 | 25 | 30 |
| 5 | 20 | 25 |
| 6 | 20 | 20 |
| 7 | 20 | 15 |
| 8 | 20 | 15 |

---

### 2. Koridor / Geçiş Alanı

Amaç: Yürüyeni yakala, duranı umursama.

| Gate | Hareket | Duran |
|----|----|----|
| 0 | 55 | 0 |
| 1 | 50 | 0 |
| 2 | 40 | 0 |
| 3 | 35 | 0 |
| 4 | 30 | 0 |
| 5 | 25 | 0 |
| 6 | 25 | 0 |
| 7 | 25 | 0 |
| 8 | 25 | 0 |

---

### 3. Balkon / Pencere Önü

Amaç: Rüzgar, perde, kuş gibi dış etkenleri yok saymak.

| Gate | Hareket | Duran |
|----|----|----|
| 0 | 60 | 0 |
| 1 | 55 | 0 |
| 2 | 50 | 0 |
| 3 | 45 | 0 |
| 4 | 40 | 0 |
| 5 | 35 | 0 |
| 6 | 35 | 0 |
| 7 | 35 | 0 |
| 8 | 35 | 0 |

---

### 4. Tavan Montajı (3–3.5 m)

Amaç: Alt alanı kapsa, kendi kendine tetiklenmesin.

| Gate | Hareket | Duran |
|----|----|----|
| 0 | 0 | 0 |
| 1 | 0 | 0 |
| 2 | 35 | 30 |
| 3 | 30 | 30 |
| 4 | 25 | 25 |
| 5 | 25 | 20 |
| 6 | 25 | 20 |
| 7 | 25 | 15 |
| 8 | 25 | 15 |

---

## İnce Ayar İpuçları

- Duran > 30 → hayalet algılama riski
- Hareket < 15 → perde / klima tetiklenir
- İlk 1–2 gate’i yüksek tutmak yanlış algıyı ciddi azaltır
- Metal kasa veya metal yüzeye çok yakın montajdan kaçınılmalı

---

## Genel Yorum

LD2410 doğru ayarlanırsa:
- Işık otomasyonu için güvenilir
- Alarm senaryolarında stabil
- Uzun süreli varlık algılamada tutarlı çalışır

Yanlış ayarlanırsa:
- Oda boşken insan var sanır
- Klimayla kavga eder
- Kullanıcıyı sensörden soğutur

Sensör değil, ayar konuşur.
