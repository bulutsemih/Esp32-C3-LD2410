# LD2410 Radar Ayar Rehberi (Gate ve Hassasiyet)

LD2410 radarını ayarlarken en kafa karıştırıcı kısım genellikle "Gate" (Kapı) mantığı ve hassasiyet değerlerinin ters çalışmasıdır. Bu rehber, ayarları doğru yapmanıza yardımcı olacaktır.

## 1. Gate (Kapı) Nedir?
Radar, mesafeyi 0.75 metrelik dilimlere böler. Her bir dilime "Gate" denir. Hangi kapının hangi mesafeye denk geldiğini bilmeniz önemlidir:

*   **Gate 0:** 0 - 0.75 Metre (Cihazın dibi)
*   **Gate 1:** 0.75 - 1.50 Metre
*   **Gate 2:** 1.50 - 2.25 Metre
*   **Gate 3:** 2.25 - 3.00 Metre
*   **Gate 4:** 3.00 - 3.75 Metre
*   **Gate 5:** 3.75 - 4.50 Metre
*   **Gate 6:** 4.50 - 5.25 Metre
*   **Gate 7:** 5.25 - 6.00 Metre
*   **Gate 8:** 6.00 - 6.75 Metre (En uzak)

## 2. Hassasiyet Değeri (Eşik Değeri) Nedir?
Girdiğiniz 0-100 arasındaki sayı bir **"Eşik" (Threshold)** değeridir.
Mantık şudur: **"Radarın gördüğü enerji, bu sayıdan büyükse 'İnsan Var' de."**

*   **DÜŞÜK Sayı (Örn: 10-20) = YÜKSEK Hassasiyet:**
    *   Çok küçük hareketleri (nefes alma, parmak oynatma) bile algılar.
    *   *Risk:* Yanlış alarm verebilir (perde kımıldaması, vantilatör vb.).
*   **YÜKSEK Sayı (Örn: 50-80) = DÜŞÜK Hassasiyet:**
    *   Sadece büyük hareketleri (yürüme, kol sallama) algılar.
    *   *Risk:* Hareketsiz durduğunuzda sizi "yok" sayabilir.

## 3. Adım Adım Nasıl Ayar Yapılır? (Pratik Yöntem)
Kütüphane kısıtlaması nedeniyle anlık enerjiyi kapı bazlı göremediğimiz için **Ana Sayfa**daki "Hareket Enerjisi" verisini referans alarak ayar yapıyoruz.

**Senaryo:** 2 metre uzaklıktaki (Gate 2) koltukta otururken radarın sizi algılamasını istiyorsunuz.

1.  **Mesafeyi Tespit Et:**
    *   Koltukta oturun ve Ana Sayfa'daki `Hareket Mesafesi`ne bakın.
    *   Örn: **210 cm** yazıyorsa, bu **Gate 2** (1.50m - 2.25m) aralığıdır.

2.  **Enerjiyi Ölç (Varken):**
    *   Koltukta hafifçe kımıldayın (normal oturma hali).
    *   Ana sayfadaki `Hareket Enerjisi`ne bakın. Diyelim ki **%45** civarında dalgalanıyor.
    *   Demek ki sizi algılaması için eşiğin **45'ten küçük** olması lazım (Örn: 30-35).

3.  **Enerjiyi Ölç (Yokken / Gürültü):**
    *   Odadan çıkın veya hareketsiz durun.
    *   Ana sayfadaki enerjiye bakın. Diyelim ki vantilatör veya perde yüzünden enerji **%15** civarında.
    *   Demek ki yanlış alarm vermemesi için eşiğin **15'ten büyük** olması lazım.

4.  **Ayarı Gir:**
    *   **Gelişmiş Ayarlar** sayfasına gidin.
    *   **Gate 2** satırını bulun.
    *   **Hareket Eşiği** kutusuna, ölçtüğümüz iki değerin ortasını yazın. (15 ile 45 arası -> **30**).
    *   "Ayarları Kaydet"e basın.

## 4. Özet Tablo

| Durum | Ne Yapılmalı? | Örnek Değişiklik |
| :--- | :--- | :--- |
| **Hayalet Algılama:** Oda boş ama radar sürekli "VAR" diyor. | İlgili kapının (genelde Gate 0, 1 veya 2) değerini **YÜKSELTİN**. | 40 -> 60 yapın. |
| **Körlük:** Odada duruyorum ama radar beni görmüyor ("YOK" diyor). | Bulunduğunuz mesafedeki kapının değerini **DÜŞÜRÜN**. | 50 -> 30 yapın. |
| **Uzağı Görmesin:** Sadece yakını görsün, arkası duvar. | Uzak kapıların (Gate 5, 6, 7, 8) değerini **100** yapın. | Gate 5-8 arası hepsini 100 yapın. |
| **Sadece Hareket:** Duran insanları görmesin. | "Duran Eşiği" değerlerini tüm kapılarda **100** yapın. | Tüm Duran Eşikler -> 100 |

---

## 5. Mesafe Sınırlama (Max Gate Ayarı)
Web arayüzündeki "Mesafe Ayarları" kısmı, radarın en fazla ne kadar uzağa bakacağını belirler. Bu ayar, hassasiyetten farklıdır; radarın "görüş mesafesini" keser.

**Örnek Senaryo: 2 Metre Mesafedeki Koltuk**
*   2 metre mesafe, **Gate 2** (1.50m - 2.25m) aralığına denk gelir.
*   Ancak koltukta geriye yaslanma veya hafif kayma durumları için bir sonraki kapıyı (**Gate 3**) sınır olarak seçmek daha güvenlidir.

**Önerilen Ayarlar:**
*   **Hareketli Max Kapı:** **3** (Yaklaşık 3.00 metreye kadar tarar)
*   **Duran Max Kapı:** **3**
*   **Kaybolma Süresi:** **5** sn

*İpucu: Eğer koltuğun hemen arkasında (örn: 2.5 metrede) hareket eden bir perde veya vantilatör varsa ve yanlış alarm veriyorsa, bu değerleri **2** yaparak radarı 2.25 metrede kör edebilirsiniz.*