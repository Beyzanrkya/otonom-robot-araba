# 4 Tekerlekli Otonom Robot Araba

Üniversite yarışmaları ve gösterimler için geliştirilmiş 4 tekerlekli otonom robot aracın ışık tarama görevi. Araç bir hat boyunca **ileri gidip geri gelerek** LDR sensörüyle ışık ölçer. Sonra **en aydınlık** ve **en karanlık** noktaları bulup bu noktalara gider.

## Görev Akışı

1. **Tarama:** Araç adım adım ileri gider ve her adımda durup ışığı ölçer.
2. **En aydınlık nokta:** Geri gelerek en aydınlık noktaya gider. LED 3 kez yavaşça yanıp söner.
3. **En karanlık nokta:** En karanlık noktaya gider. LED 5 kez hızlıca yanıp söner.
4. **Dönüş:** Başlangıç noktasına geri döner.

Ölçümler Seri Monitör'den (9600 baud) takip edilebilir.

## Donanım

| Parça | Adet |
|---|---|
| Arduino Uno | 1 |
| L298N motor sürücü | 1 |
| DC motor + tekerlek | 4 |
| LDR (ışık sensörü) | 1 |
| 10 kΩ direnç | 1 |
| LED + 220 Ω direnç | 1 |

## Bağlantılar

| Arduino | Bağlandığı yer |
|---|---|
| D5 (PWM) | L298N ENA (sol motorlar hız) |
| D7, D8 | L298N IN1, IN2 (sol motorlar yön) |
| D6 (PWM) | L298N ENB (sağ motorlar hız) |
| D9, D10 | L298N IN3, IN4 (sağ motorlar yön) |
| A0 | LDR ile 10 kΩ direncin birleşim noktası |
| D13 | LED |

**LDR bağlantısı (gerilim bölücü):** 5V → LDR → A0 → 10 kΩ → GND. Bu bağlantıda ışık arttıkça okunan değer de artar.

## Ayarlar

`ldr_tarama/ldr_tarama.ino` dosyasının başındaki değerlerle davranış değiştirilebilir:

- `MOTOR_HIZI`: Motor hızı (0–255)
- `TOPLAM_ADIM`: Tarama boyunca yapılacak ölçüm sayısı
- `ADIM_SURESI`: Her adımda motorların çalışma süresi (ms)
- `ORNEK_SAYISI`: Her ölçümde ortalaması alınan örnek sayısı

> Araçta enkoder olmadığı için konum, sabit süreli adımlarla takip edilir. Zemin ve batarya durumuna göre `ADIM_SURESI` değerini ayarlamak gerekebilir.

## Yükleme

1. Arduino IDE ile `ldr_tarama/ldr_tarama.ino` dosyasını aç.
2. Kart olarak **Arduino Uno**'yu seç ve kodu yükle.
3. Aracı başlangıç noktasına koy. Kod 2 saniye sonra görevi başlatır.
