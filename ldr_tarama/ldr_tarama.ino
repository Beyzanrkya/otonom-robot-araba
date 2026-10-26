/*
  TEKNOFEST Otonom Araç - LDR ile En Aydınlık / En Karanlık Noktayı Bulma

  Çalışma mantığı:
    1. TARAMA   : Araç adım adım ileri gider, her adımda durup LDR ile ışığı ölçer.
                  En yüksek ve en düşük ışık değerinin hangi adımda olduğunu kaydeder.
    2. AYDINLIK : Geri gelerek en aydınlık noktaya gider, LED'i 3 kez yavaşça yakıp söndürür.
    3. KARANLIK : En karanlık noktaya gider (ileri ya da geri), LED'i 5 kez hızlıca yakıp söndürür.
    4. DÖNÜŞ    : Başlangıç noktasına geri döner ve durur.

  Donanım: Arduino Uno, L298N motor sürücü, 4 DC motor, LDR (+10k direnç), LED
  Not: Araçta enkoder olmadığı için konum, sabit süreli adımlarla (zamana göre) takip edilir.
*/

// ---------------- Pin tanımları ----------------
// L298N - Sol motorlar
const int ENA = 5;   // PWM hız
const int IN1 = 7;
const int IN2 = 8;
// L298N - Sağ motorlar
const int ENB = 6;   // PWM hız
const int IN3 = 9;
const int IN4 = 10;

const int LDR_PIN = A0;   // LDR gerilim bölücü çıkışı
const int LED_PIN = 13;   // Durum LED'i

// ---------------- Ayarlar ----------------
const int MOTOR_HIZI     = 150;  // 0-255
const int TOPLAM_ADIM    = 10;   // Tarama boyunca kaç noktada ölçüm yapılacak
const int ADIM_SURESI    = 400;  // Bir adımda motorların çalışma süresi (ms)
const int BEKLEME_SURESI = 300;  // Durduktan sonra ölçümden önce bekleme (ms)
const int ORNEK_SAYISI   = 10;   // Her ölçümde alınan örnek sayısı (ortalama için)

// ---------------- Durum değişkenleri ----------------
int olcumler[TOPLAM_ADIM];
int enAydinlikAdim = 0;
int enKaranlikAdim = 0;
int mevcutAdim = 0;  // Aracın şu an bulunduğu adım

// ================= Motor fonksiyonları =================
void motorlariDurdur() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void ileriGit() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, MOTOR_HIZI);
  analogWrite(ENB, MOTOR_HIZI);
}

void geriGit() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, MOTOR_HIZI);
  analogWrite(ENB, MOTOR_HIZI);
}

// Aracı bir adım ileri (+1) veya geri (-1) götürür
void birAdim(int yon) {
  if (yon > 0) ileriGit(); else geriGit();
  delay(ADIM_SURESI);
  motorlariDurdur();
  delay(BEKLEME_SURESI);
  mevcutAdim += yon;
}

// Aracı istenen adıma götürür
void adimaGit(int hedef) {
  while (mevcutAdim < hedef) birAdim(+1);
  while (mevcutAdim > hedef) birAdim(-1);
}

// ================= Sensör / LED =================
// Gürültüyü azaltmak için birkaç ölçümün ortalamasını alır
int isikOku() {
  long toplam = 0;
  for (int i = 0; i < ORNEK_SAYISI; i++) {
    toplam += analogRead(LDR_PIN);
    delay(5);
  }
  return toplam / ORNEK_SAYISI;
}

void ledYakSondur(int kez, int sure) {
  for (int i = 0; i < kez; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(sure);
    digitalWrite(LED_PIN, LOW);
    delay(sure);
  }
}

// ================= Görevler =================
void tara() {
  Serial.println("--- TARAMA BASLADI ---");
  for (int adim = 0; adim < TOPLAM_ADIM; adim++) {
    if (adim > 0) birAdim(+1);
    olcumler[adim] = isikOku();

    if (olcumler[adim] > olcumler[enAydinlikAdim]) enAydinlikAdim = adim;
    if (olcumler[adim] < olcumler[enKaranlikAdim]) enKaranlikAdim = adim;

    Serial.print("Adim ");
    Serial.print(adim);
    Serial.print(" -> isik: ");
    Serial.println(olcumler[adim]);
  }

  Serial.print("En aydinlik nokta: adim ");
  Serial.print(enAydinlikAdim);
  Serial.print(" (");
  Serial.print(olcumler[enAydinlikAdim]);
  Serial.println(")");
  Serial.print("En karanlik nokta: adim ");
  Serial.print(enKaranlikAdim);
  Serial.print(" (");
  Serial.print(olcumler[enKaranlikAdim]);
  Serial.println(")");
}

// ================= Arduino =================
void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  motorlariDurdur();

  Serial.begin(9600);
  delay(2000);  // Aracı yere koymak için süre

  // 1. Tarama
  tara();

  // 2. En aydınlık noktaya git
  Serial.println("--- EN AYDINLIK NOKTAYA GIDILIYOR ---");
  adimaGit(enAydinlikAdim);
  ledYakSondur(3, 500);

  // 3. En karanlık noktaya git
  Serial.println("--- EN KARANLIK NOKTAYA GIDILIYOR ---");
  adimaGit(enKaranlikAdim);
  ledYakSondur(5, 150);

  // 4. Başlangıca dön
  Serial.println("--- BASLANGICA DONULUYOR ---");
  adimaGit(0);
  motorlariDurdur();
  Serial.println("--- GOREV TAMAMLANDI ---");
}

void loop() {
  // Görev setup() içinde bir kez çalışır. Tekrar için Arduino'nun reset tuşuna basın.
}
