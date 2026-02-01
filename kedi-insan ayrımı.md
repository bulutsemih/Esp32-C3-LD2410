# LD2410 Kedi – İnsan Ayrımı İnce Ayar Rehberi

Bu doküman LD2410 radar varlık sensöründe **kedi / küçük hayvan** tetiklemelerini azaltmak,
insan algılamasını korumak için kullanılan ayar ve mantıkları içerir.

> Gerçek: LD2410 bir radar sensördür, kamera değildir.
> Boy, hız ve mikroyansımaya bakar. Tür bilmez.

---

## Temel Ayrım Mantığı

| Özellik | İnsan | Kedi |
|------|------|------|
| Radar kesit alanı | Büyük | Küçük |
| Hareket paterni | Gövde + kol + bacak | Alçak, hızlı |
| Duran enerji | Yüksek | Çok düşük |
| Mesafe etkisi | 1–6 m net | 0–2 m güçlü |

Bu yüzden:
- **Duran algılama** en kritik filtredir
- **Yakın gate’ler** en riskli bölgedir
- Kediler çoğunlukla **Gate 0–2** aralığında sorun çıkarır

---

## Altın Kurallar

1. **Gate 0–1 Duran = 0**
2. **Gate 0–2 Hareket eşiği yüksek**
3. Uzak gate’lerde (3+) insan zaten baskın
4. Duran eşiği 30 üstü → hayalet alarm riski

---

## Önerilen Ayarlar (Kedi Filtreli Oda)

| Gate | Mesafe | Hareket | Duran |
|----|----|----|----|
| 0 | 0.0 m | 60 | 0 |
| 1 | 0.75 m | 55 | 0 |
| 2 | 1.5 m | 45 | 15 |
| 3 | 2.25 m | 30 | 30 |
| 4 | 3.0 m | 25 | 30 |
| 5 | 3.75 m | 20 | 25 |
| 6 | 4.5 m | 20 | 20 |
| 7 | 5.25 m | 20 | 15 |
| 8 | 6.0 m | 20 | 15 |

---

## Bu Ayarlarla Ne Olur?

### 🐈 Kedi Senaryosu
- Yerde dolaşır → çoğu zaman algılanmaz
- Hızlı geçiş → kısa hareket algısı olabilir
- Oturur → duran algılama yok

### 🧍 İnsan Senaryosu
- Ayakta durur → algılanır
- Oturur → algı devam eder
- Yavaş hareket → algı düşmez

---

## Neden İşe Yarıyor?

- Kedinin **mikro hareket enerjisi**, Gate 0–2’de yüksek eşiği geçemez
- İnsan gövdesi Gate 3+’ta güçlü yansıma üretir
- Duran algılama kedinin nefesini yok sayar

---

## Ek Filtre (Yazılımsal – Şiddetle Önerilir)

### Zaman Filtresi
> 3–5 saniyeden kısa algı = yok say

Kediler genelde:
- Girip çıkar
- Uzun süre sabit kalmaz

İnsanlar kalır.

---

## Home Assistant Mantık Örneği

- Hareket algılandı
- 5 saniye boyunca kesintisiz devam ediyorsa → **occupied**
- Aksi halde → **ignore**

Bu filtre tek başına kediyi %50 daha azaltır.

---

## Yapılmaması Gerekenler

❌ Duran eşiği 20 altına çekmek  
❌ Gate 0–1 çok hassas yapmak  
❌ Sensörü yere 50–80 cm monte etmek  
❌ Kedinin mama alanına doğrultmak  

---

## Fiziksel Montaj Tavsiyesi

- Yükseklik: **1.8 – 2.2 m**
- Hafif aşağı bakmalı
- Zemini değil gövdeyi görmeli
- Metal yüzeylerden uzak

---

## Gerçekçi Beklenti

| Senaryo | Başarı |
|------|------|
| Tek kedi | %85–90 |
| 2–3 kedi | %70–80 |
| Kedi + insan aynı anda | %90 insan doğru |
| Kedi zıplarsa | Kaçınılmaz tetik |

Radar kandırılır ama bu kadar.

---

## Son Not

LD2410:
- Doğru ayarla ev otomasyonu için mükemmel
- Yanlış ayarla paranormal aktivite dedektörü

Kediler suçlu değil. Ayarlar konuşur.
