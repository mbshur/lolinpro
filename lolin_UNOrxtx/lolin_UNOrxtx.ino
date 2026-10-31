#include <Servo.h> // Servo nesnelerini dinamik eklemek için standart kütüphane


// --- ⚙️ DİNAMİK PİN TANIMLAMA SÖZLÜKLERİ (UNO RAM DOSTU SABİTLER) ---
#define MAX_MODUL 5 // Her modül tipinden en fazla kaç tane tanımlanabilir

// 1. STEP MOTOR HAFIZASI (Örn: STP=SM:2,3;)
String stepIsimleri[MAX_MODUL];
int stepPin1[MAX_MODUL];
int stepPin2[MAX_MODUL];
int toplamStep = 0;

// --- ⚙️ STEP MOTOR ARKA PLAN YÖNETİMİ ---
unsigned long stepSonAdimZamani[MAX_MODUL] = {0};
long stepKalanAdim[MAX_MODUL] = {0}; // Kaç adım atılacağı (Pozitif veya Negatif yön)
int stepAktifPin[MAX_MODUL] = {0};    // Hangi pinin tetikleneceği
// Her motorun kendi hızını (mikrosaniye) tutan yeni dizimiz:
unsigned long stepAdimGecikmesi[MAX_MODUL] = {500}; // Varsayılan: 500us


String servoIsimleri[MAX_MODUL];  // Servoların isimlerini (Örn: SR0) tutan dizi
int servoPin[MAX_MODUL];          // Servoların bağlı olduğu pin numaralarını tutan dizi
// (Önceki adımda eklediğimiz diğer değişkenler de burada durmalı:)
Servo servoNesneleri[MAX_MODUL];
bool servoBaglandi[MAX_MODUL] = {false};



int servoHedefAci[MAX_MODUL] = {90};
float servoAnlikAci[MAX_MODUL] = {90};
unsigned long servoSonGuncellemeZamani[MAX_MODUL] = {0};
int servoAdimGecikmesi[MAX_MODUL] = {15}; 
const float SERVO_ADIM_HIZI = 1.0;
int toplamServo=0;


// 3. PWM ÇIKIŞ HAFIZASI (Örn: PWM=RLED:6;)
String pwmIsimleri[MAX_MODUL];
int pwmPin[MAX_MODUL];
int toplamPwm = 0;

// --- 💡 PWM YUMUŞAK GEÇİŞ (FADE) YÖNETİMİ ---
int pwmHedefDeger[MAX_MODUL] = {0};
float pwmAnlikDeger[MAX_MODUL] = {0};
unsigned long pwmSonGuncellemeZamani[MAX_MODUL] = {0};
// Her PWM çıkışının kendi hızını tutan yeni dizimiz:
int pwmAdimGecikmesi[MAX_MODUL] = {5}; // Varsayılan: Her adımda 5ms bekleme



// 4. ANALOG GİRİŞ HAFIZASI (Örn: ANG=POT1:A0;)
String analogIsimleri[MAX_MODUL];
int analogPin[MAX_MODUL]; // A0, A1 gibi pinler arka planda tam sayıdır (14, 15 vb.)
int toplamAnalog = 0;

// 5. DİJİTAL GİRİŞ HAFIZASI (Örn: DI=SW1:7,0)
String diIsimleri[MAX_MODUL];
int diPin[MAX_MODUL];
int diNormalDurum[MAX_MODUL]; // Anahtarın arıza olmayan, normal hali (0 veya 1)
int toplamDi = 0;

// 6. DİJİTAL ÇIKIŞ HAFIZASI (Örn: DO=ROLE:8)
String doIsimleri[MAX_MODUL];
int doPin[MAX_MODUL];
int toplamDo = 0;

const int ledPin = 13;   // Temsili animasyon pini (Dahili led)


//donanımsal seri portu (Hardware Serial yani 
// D0/RX ve D1/TX pinlerini) kullanıyoruz,

String CHZadi ="";

String rxdata="";
String txdata="";

int raporzamanlayici;
unsigned long raporsayaci;
void setup() {
  // Donanımsal seri port 19200 baud hızında başlatılıyor (Halkadaki tek hız)
  //Serial.begin(19200);   
  Serial.begin(38400);  
  pinMode(ledPin, OUTPUT);

}

void loop() {

  // 🎯 Her döngüde sensörleri asenkron olarak kontrol et ve zamanı gelince ESP'ye bas!
  updinput();

  // Sizin mevcut haberleşme ve kontrol motorlarınız aynen çalışmaya devam eder
  slaveRxvoid();
  CHZkontrol();
  if(raporzamanlayici>0)raporzamanlayicivoid();
  slaveTxvoid();
  
  // 3. 🚨 BLOKE ETMEYEN ARKA PLAN MOTORLARI
  servoYumusakHareketYonetimi(); // Servoları süzerek götürür
  stepArkaPlanYonetimi();        // Step motor adımlarını milisaniyede kaçırmadan atar
  pwmYumusakGecisYonetimi();     // LED'leri yumuşakça yakıp söndürür
}




