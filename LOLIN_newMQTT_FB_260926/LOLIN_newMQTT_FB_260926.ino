// ESP8266 LOLİN
// Flash size 4M(FS:1MB OTA:~1019KB)
// MMU: "16KB cache 48kb iram 2 heap shared

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266mDNS.h>  // mDNS kütüphanesini dahil ediyoruz

//#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "FS.h"
#include "LittleFS.h"
#include <MQTT.h>
#include <Servo.h>
#include <DHT.h>
#include "Melody.h"


WiFiClient mqttnet;
MQTTClient mqttclient;
String MQTTip = "";



//String USER_EMAIL1 = "";  // "asd"     @ işareti sonradan eklenecek
//String USER_EMAIL2 = "";  //"gmail.com"

/* 2. Define the API Key */
//String API_KEY = "";  // "AIzaSyDTMhGs_ISD4WKmJrCxw35rqv-bo34ZdYI";

/* 3. Define the RTDB URL */
String DATABASE_URL = "https://kev1-e3fbe-default-rtdb.firebaseio.com/";  // "https://esp-v4-default-rtdb.firebaseio.com"; //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app



/* 4. Define the user Email and password that alreadey registerd or added in your project */
//String USER_EMAIL = "";     // "asadafag@gmail.com";
//String USER_PASSWORD = "";  // "bebedede14";




bool fbisleniyor = false;
String YOL = "";
String errorlog;
int CHZz;
bool psco;
bool psci;

// firebaseServer isem
String FBSERVER;
unsigned long rutingonder10s = millis();
/*
bool BenFbServerim=false;
String espna[20];
String espma[20];
String esppa[20];
String esppi[20];
String esprs[20];
*/
//////////////////////



unsigned long reConnectsayac = millis();


bool WiFiAP = true;  // Do yo want the ESP as AP?
WiFiServer httpserver(80);
String header;
String headerold;
String creator;
String htyolla;
String esphostnameOnek = "";
String esphostname = "esp-bos";
//unsigned long harcananzaman;
String progmsg;


unsigned long timereski = millis();



String ssid = "";
String pass = "";

bool rescanwifi = false;

bool htpcldis = false;



//bool aut[5]=false;
String unme;
String pwrd;
String capt;
unsigned long logintimeout;
int aut;
int logintimeoutmax = 240000;


String cihazmac = "";       // Dosyadan okunan veya üretilen aktivasyon kodu
bool cihazorjinal = false;  // Cihazın durum bayrağı ("EVET" veya "HAYIR")


int PIN_TONE;
String Host;

int sayfayenile = 0;

String VRP[5];
int pinsayisi = 9;
// pin Adı|pinmode|pin baslangic degeri|pin değeri|pin degerleri|Pin label"

bool pinayarchg = false;
bool habpchg = false;
String pinsatir[10];
String pinname[10];
String pinmode[10];
String pinsignaltype[10];
String pinminvalue[10];
String pinval[10];
String PinState[10];
String ePinState[10];
bool acildeyim[10];
String fbPinState[10];
String pinmaxvalue[10];
String acilseviyesi[10];
String acildeger[10];
String pinlabel[10];

bool rxtxvar = false;
String rxtampon = "";
String rxdata = "";
String txdata = "";

String ACL = "9";
String eACL = "9";
bool ACLilanciyim = false;
unsigned long acltekrar = millis();
unsigned long aclsor = millis();

String Abonelik;
//String pindurumrec;
//bool pindurumrecyap;
int dhtsayac = 1;
// 10 elemanlı boş bir pointer dizisi. Başlangıçta hepsi NULL (boş)
DHT* DHTA[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

// 10 adet pin için Servo nesnesi işaretçi dizisi (Pointer Array)
Servo* myservo[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
bool servovar[10] = { false };  // Hangi servonun aktif olduğunu takip eden bayrak

int hcsrT[10];
int hcsrE[10];
int edistance[10];
int bestursay = 0;
bool hcsrloopvar;
unsigned long hcsrlooptimer;
String tempstr = "";
String humstr = "";



String pinayar;
String pinayartmp;
String Program;

// PROGRAM İÇİN ////////////////////////
String satirp;
String degis[20];
String degdeg[20];
String tmris[10];
String tmrdeg[10];
// 0-10  PİN     11-20 degisken 


////////////////////////////////////////

//DHT dht(DHTPIN, DHTTYPE);
//DHT DHTA(D8, DHT11);


String yazi;



File dosya;


String pinayarerrorlog = "";
String programerrorlog = "";
String RXTXerrorlog = "";
//int pinayarerrorlogsatir=0;
String programdata;

bool uzunMetrajVar = false;    // Uzun metraj modu aktif mi?
String uprogramdata = "";      // Uzun metraj program verisi
String uprogramerrorlog = "";  // Hata logları için


int habp = -2;
int ehabp;
bool fben;

String fberror = "";





String htServerip;

String edegisenler;
String degisenler;


//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
//ESP8266WebServer server(8080);
/*
void handleRoot() {

  dosyaokumyssidname();
  IPAddress lip = WiFi.localIP();
  String lipStr = String(lip[0]) + '.' + String(lip[1]) + '.' + String(lip[2]) + '.' + String(lip[3]);
  String gd = YOL + "[" + esphostname + "]" + lipStr;
  server.send(200, "text/plain", gd);
}

void handleger() {

  dosyaokumyssidname();
  IPAddress lip = WiFi.localIP();
  String lipStr = String(lip[0]) + '.' + String(lip[1]) + '.' + String(lip[2]) + '.' + String(lip[3]);
  String gd = YOL + "[" + esphostname + "]" + lipStr;
  server.send(200, "text/plain", gd);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }

  Serial.println(message);
  String seriptm = server.uri();
  String serip = Karakterduzeltfunc(seriptm);
  if (serip.indexOf("/ser:") > -1) {
    dosyaokumyssidname();
    IPAddress lip = WiFi.localIP();
    String lipStr = String(lip[0]) + '.' + String(lip[1]) + '.' + String(lip[2]) + '.' + String(lip[3]);
    String gd = YOL + "[" + esphostname + "]" + lipStr;
    server.send(200, "text/plain", gd);

    serip = serip.substring(serip.indexOf("ser:") + 4, serip.length());
    Serial.println("Server geldi          ");
    Serial.println(serip);
    htserverkaydet(serip);
  } else server.send(404, "text/plain", message);
}
*/

bool LED_BUILTIN_devredisi = false;

bool pinlerdagitildi = false;

String Pinler;
String macadrparse[10];






int getGPIOPin(String pinStr) {
  pinStr.trim();         // Boşlukları temizler
  pinStr.toUpperCase();  // Küçük harf girilse bile (a0 -> A0) eşleşmesini sağlar

  // Analog Pin Eşleşmeleri
  if (pinStr == "A0") return A0;
#ifdef A1
  if (pinStr == "A1") return A1;  // Kartınızda A1, A2 vb. varsa otomatik eklenir
  if (pinStr == "A2") return A2;
  if (pinStr == "A3") return A3;
#endif

  // Dijital Pin Eşleşmeleri
  if (pinStr == "D0") return D0;
  if (pinStr == "D1") return D1;
  if (pinStr == "D2") return D2;
  if (pinStr == "D3") return D3;
  if (pinStr == "D4") return D4;
  if (pinStr == "D5") return D5;
  if (pinStr == "D6") return D6;
  if (pinStr == "D7") return D7;
  if (pinStr == "D8") return D8;

  // Eğer başında harf yoksa ve direkt sayısal bir GPIO stringi ise ("4" gibi)
  if (pinStr.length() > 0 && isDigit(pinStr.charAt(0))) {
    return pinStr.toInt();
  }

  return -1;  // Hatalı veya bilinmeyen pin ismi durumunda -1 döner
}












void dosyaOkupinayar() {
  String strtmp;
  dosya.close();
  //LittleFS.remove("/pinayar.txt");

  dosya = LittleFS.open("/pinayar.txt", "r");
  if (dosya) {
    for (int p = 0; p < 11; p++) {
      String gp = dosya.readStringUntil('\n');
      if (gp.length() < 5) {
        //pinsayisi = p + 1;
        break;
      }
    }
  }
  dosya.close();
  delay(20);
  dosya = LittleFS.open("/pinayar.txt", "r");
  if (dosya) {
    String gecicipinayar = dosya.readString();
    pinayartmp = gecicipinayar.substring(0, gecicipinayar.length());

    pinayartmp.replace("\n\n", "");

    int contpih = 0;
    int n = 0;

    while ((n = pinayartmp.indexOf('|', n)) != -1) {
      n++;
      contpih++;
    }

    if (contpih % 8 != 0) {
      pinayarerrorlog += "pinayar hatalı";
      pinayar = "";
      return;
    } else {
      pinayar = pinayartmp;
    }
    dosya.close();

    if (programdata.indexOf("RXTX") != -1 && programdata.indexOf("//RXTX;") < 0) rxtxvar=true;

    strtmp = pinayar;
    Serial.println("dosyaokupinayar.");
    Serial.println(pinayar);

    if (strtmp.length() > 0) {
      for (int x = 0; x < pinsayisi + 1; x++) {
        pinname[x] = "";
        pinmode[x] = "";
        pinsignaltype[x] = "";
        pinminvalue[x] = "";
        pinval[x] = "";
        PinState[x] = "";
        ePinState[x] = "";
        //fbPinState[x] = "";
        pinmaxvalue[x] = "";
        pinlabel[x] = "";
        acilseviyesi[x] = "";
        acildeger[x] = "";
        hcsrT[x] = -1;
        hcsrE[x] = -1;
      }





      hcsrloopvar = false;
      yield();

      for (int x = 0; x < pinsayisi + 1; x++) {



        if (strtmp.length() < 3) break;

        if (strtmp.indexOf("//") != 0) {

          String pinnametm = strtmp.substring(0, strtmp.indexOf("|"));

          int b = pinnametm.length() + 1;
          char cvc[b];
          pinnametm.toCharArray(cvc, b);  // pinname 0A ise atla
          if ((int)cvc[0] != 10) {

            int pinismiint;
            if (pinnametm.length() > 1) {
              pinismiint = pinnametm.substring(1, 2).toInt();
              if (pinnametm.substring(0, 1) == "A") pinismiint += 9;
            } else pinismiint = pinnametm.toInt();

            //Serial.println(pinnametm);
            //Serial.println(pinismiint);

            x = pinismiint;
            //pinsatir[x] = strtmp.substring(0,strtmp.indexOf("\n"));
            if(strtmp.indexOf("D4")>-1)LED_BUILTIN_devredisi=true;
            //Serial.println(j);

            pinname[x] = pinnametm.substring(0, pinnametm.indexOf("|"));
            strtmp = strtmp.substring(strtmp.indexOf("|") + 1, strtmp.length());

            pinmode[x] = strtmp.substring(0, strtmp.indexOf("|"));
            strtmp = strtmp.substring(strtmp.indexOf("|") + 1, strtmp.length());

            pinsignaltype[x] = strtmp.substring(0, strtmp.indexOf("|"));
            strtmp = strtmp.substring(strtmp.indexOf("|") + 1, strtmp.length());


            pinminvalue[x] = strtmp.substring(0, strtmp.indexOf("|"));
            strtmp = strtmp.substring(strtmp.indexOf("|") + 1, strtmp.length());

            if (pinlerdagitildi == false) {
              if (PinState[x] == "") {
                if (pinsignaltype[x] == "PWM") {
                  int asd = strtmp.substring(0, strtmp.indexOf("|")).toInt();
                  if (asd < 0) PinState[x] = "0";
                  if (asd > 255) PinState[x] = "255";  // String olduğu için tırnak içine alındı
                }
                if (pinsignaltype[x] == "SER") {
                  int asd = strtmp.substring(0, strtmp.indexOf("|")).toInt();
                  if (asd < 0) PinState[x] = "0";
                  if (asd > 180) PinState[x] = "180";  // String olduğu için tırnak içine alındı
                } else {
                  PinState[x] = strtmp.substring(0, strtmp.indexOf("|"));
                }

                if (pinsignaltype[x] == "BUZ") {
                  int gpio = getGPIOPin(pinname[x]);
                  PIN_TONE = gpio;
                }

                if (pinsignaltype[x] == "HCE") {
                  int gpio = getGPIOPin(pinname[x]);
                  hcsrE[x] = gpio;
                  hcsrloopvar = true;
                  int gpio2 = getGPIOPin(pinname[x]);
                  hcsrT[x] = gpio2;
                }

                // 🎯 =================================================================
                // 🎯 SERVO MOTOR İÇİN DİNAMİK NESNE YARATMA (DHT Mantığının Aynısı)
                // -----------------------------------------------------------------
                if (pinmode[x] == "OUT" && pinsignaltype[x] == "SER") {
                  // Eğer o pinde eski bir servo nesnesi kalmışsa önce hafızayı tamamen boşaltalım (Re-init koruması)
                  if (myservo[x] != NULL) {
                    myservo[x]->detach();  // Önce pinden ayır
                    delete myservo[x];     // Hafızadan sil
                    myservo[x] = NULL;     // İşaretçiyi boşa çıkar
                  }

                  int8_t fizikselPin = getGPIOPin(pinname[x]);
                  pinMode(fizikselPin, OUTPUT);

                  // İşte sihirli nesneleştirme satırı: Yeni servo nesnesini dinamik yaratıyoruz!
                  myservo[x] = new Servo();
                  myservo[x]->attach(fizikselPin);  // Pointer olduğu için '->' kullanılır

                  servovar[x] = true;  // Servonun başarıyla kurulduğunu onayla
                  Serial.printf_P(PSTR("[SERVO] Pin %s dinamik nesne olarak basariyla kuruldu.\n"), pinname[x].c_str());
                }

                // Eğer eski bir DHT nesnesi varsa hafızayı boşaltalım (re-init koruması)
                if (pinmode[x] == "INP" && pinsignaltype[x].indexOf("DHT") == 0) {
                  if (DHTA[x] != NULL) {
                    delete DHTA[x];
                    DHTA[x] = NULL;
                  }
                  // Eğer bu pin bir DHT sensörüyse nesneyi buracıkta yaratıyoruz:
                  if (pinsignaltype[x].indexOf("DHT") == 0) {
                    uint8_t DHT_TYPE = DHT11;
                    if (pinsignaltype[x] == "DHT1") DHT_TYPE = DHT11;
                    else if (pinsignaltype[x] == "DHT2") DHT_TYPE = DHT12;
                    else if (pinsignaltype[x] == "DHT3") DHT_TYPE = DHT21;
                    else if (pinsignaltype[x] == "DHT4") DHT_TYPE = DHT22;
                    else pinayarerrorlog = "DHT pinsignaltype yanlış " + pinsignaltype[x] + " olamaz. DHT11 için DHT1, DHT12 için DHT2, DHT21 için DHT3, DHT22 için DHT4 yazın.";

                    DHTA[x] = new DHT(getGPIOPin(pinname[x]), DHT_TYPE);
                    DHTA[x]->begin();
                  }
                }
              }
            }

            pinval[x] = strtmp.substring(0, strtmp.indexOf("|"));
            strtmp = strtmp.substring(strtmp.indexOf("|") + 1, strtmp.length());
            PinState[x] = pinval[x];

            pinmaxvalue[x] = strtmp.substring(0, strtmp.indexOf("|"));
            strtmp = strtmp.substring(strtmp.indexOf("|") + 1, strtmp.length());


            acilseviyesi[x] = strtmp.substring(0, strtmp.indexOf("|"));
            strtmp = strtmp.substring(strtmp.indexOf("|") + 1, strtmp.length());

            acildeger[x] = strtmp.substring(0, strtmp.indexOf("|"));
            strtmp = strtmp.substring(strtmp.indexOf("|") + 1, strtmp.length());
          }
        }

        //Serial.print("label bu");Serial.println(strtmp);

        if (strtmp.indexOf("\n") > -1) {
          pinlabel[x] = strtmp.substring(0, strtmp.indexOf("\n"));
          strtmp = strtmp.substring(strtmp.indexOf("\n") + 1, strtmp.length());
        } else {
          pinlabel[x] = strtmp.substring(0, strtmp.length());
          strtmp = "";
        }

        if (strtmp.length() < 5) { break; }
      }

      pinlerdagitildi == true;  //dosyaokupindurum();
    }
  }
  updateinput();
}

void dosyaYazpinayar() {

  reConnectsayac = millis();

  // HTML'den gelen pinayartmp verisini düzeltiyoruz
  pinayartmp = Karakterduzeltfunc(pinayartmp);
  pinayartmp.replace("\n\n", "\n");  // Çift satır boşluklarını teke indir
  pinayartmp.trim();

  String yeniPinAyarMetni = "";  // Yeniden inşa edeceğimiz güvenli metin
  int baslangicIdx = 0;
  int bitisIdx = 0;
  bool formatHatasiVar = false;

  // 1. ADIM: Metni satır satır bölerek tek tek inceliyoruz
  while ((bitisIdx = pinayartmp.indexOf('\n', baslangicIdx)) != -1 || baslangicIdx < pinayartmp.length()) {

    // Eğer son satırda \n yoksa metnin sonuna kadar al
    if (bitisIdx == -1) bitisIdx = pinayartmp.length();

    String satir = pinayartmp.substring(baslangicIdx, bitisIdx);
    satir.trim();

    if (satir.length() > 0) {
      // Bu satırdaki | karakterlerini sayalım
      int satirPipeSayisi = 0;
      for (int i = 0; i < satir.length(); i++) {
        if (satir[i] == '|') satirPipeSayisi++;
      }

      // KESİN KONTROL: Her bir satırda tam olarak 8 tane | olmak zorunda!
      if (satirPipeSayisi != 8) {
        formatHatasiVar = true;
        break;  // Bir satır bile bozuksa döngüden çık
      }
      pinayarerrorlog = "";

      // 2. ADIM: Çakışma Kontrolü (Sadece bu geçerli satır için)
      if (programdata.indexOf("RXTX") != -1 && programdata.indexOf("//RXTX;") < 0) {
        // Eğer satırda D6 veya D7 geçiyorsa ve başında zaten // yoksa engelle
        if ((satir.indexOf("D6") != -1 || satir.indexOf("D7") != -1) && satir.indexOf("//") != 0) {
          pinayarerrorlog = "Pinayar hatası: D6 veya D7 kullanılamaz! Program kısmında RXTX; tanımlı.";
          satir = "//" + satir;  // Tüm satırı güvenle yorum satırına çeviriyoruz
        }
      }

      // Temiz veya düzeltilmiş satırı yeni metne ekle
      yeniPinAyarMetni += satir + "\n";
    }

    if (bitisIdx == pinayartmp.length()) break;
    baslangicIdx = bitisIdx + 1;
  }

  // 3. ADIM: Format Hatası Varsa Kaydetmeden Çık
  if (formatHatasiVar) {
    pinayarerrorlog = "pinayar hatalı: Satırlardaki parametre sayısı eksik veya fazla!";
    return;
  }

  // 4. ADIM: Değişiklik Varsa LittleFS'e Güvenle Yaz (Flash Ömrünü Korur)
  if (yeniPinAyarMetni != pinayar) {
    pinayar = yeniPinAyarMetni;

    dosya.close();
    LittleFS.remove("/pinayar.txt");
    dosya = LittleFS.open("/pinayar.txt", "w+");

    if (dosya) {
      dosya.print(pinayar);
      dosya.close();
      Serial.println(F("--- Yeni Pin Ayarları Kaydedildi ---"));
      Serial.println(pinayar);
    }

    if (habp == 2) pinayarchg = true;
    setup2();
    programrun();
  }
}



/*
void esp01() {
  // MQTT OLACAK FİREBASE YOK RESET ÇEKİP DURMASIN.
  // MQTT İLE MESAJ YOLLAMAYA HAZIR.
  // 192.168.2.105
  //http://192.168.4.1/ssidset?ssid=Zyxel&pass=bebedede14
  //
  // esp role alttaki direnci sök.   bir de 10k ile 3v3 ü aşırt 3. bacağına bağla
  // bordun üstünden bakarken
  //
  //  /-10k--\
//|*   *    *    *        5v GND    role çıkışlar
  //|
  //|*   *    *    *
  //|
  //|    ESP8266
  //|
  //|     anten                     ROLE
  //|_______________                   |
  //                                   |
  //     5v>3.3v                       |
  // GPIO0 = 0      int role=0         |
  //       R1\                         |
  //          |-base-- Transistor -3v  |
  //       R2/           |_____________|
  //led
  //      R2 yi sök


  Pin[0] = 0;
  Pin[2] = 2;
}

void espLolin() {
  //Led = D4;

  Pin[0] = D0;
  Pin[1] = D1;
  Pin[2] = D2;
  Pin[3] = D3;
  Pin[4] = D4;
  Pin[5] = D5;
  Pin[6] = D6;
  Pin[7] = D7;
  Pin[8] = D8;
  Pin[9] = A0;
}
*/


void setup2() {

  dosyaOkupinayar();

  if (pinayar.length() > 3) {
    dosyaOkuprogram();
    String strtmp = programdata;
    strtmp.toUpperCase();

    /*
    if(pinlerterslendi==false)
    {
      for (int x = 0; x < pinsayisi + 1; x++) {
        if (pinmode[x] == "OUT" & pinsignaltype[x] == "DIG") {
          bool yildizli; if(pinlabel[x].indexOf("*")+1==pinlabel[x].length())yildizli=true;else yildizli=false;
                     if(yildizli==true){
                       if(PinState[x]=="1") PinState[x] = "0"; else PinState[x] = "1";
                       }

          if (PinState[x] == "0.00" || PinState[x] == "0" || PinState[x] == "LOW" || PinState[x] == "OFF" || PinState[x] == "") {
            if (yildizli == false) digitalWrite(getGPIOPin(pinname[x]), LOW);
            else digitalWrite(getGPIOPin(pinname[x]), HIGH);
          } else if (PinState[x] == "1.00" || PinState[x] == "1" || PinState[x] == "HIGH" || PinState[x] == "ON") {
            if (yildizli == false) digitalWrite(getGPIOPin(pinname[x]), HIGH);
            else digitalWrite(getGPIOPin(pinname[x]), LOW);
          }
        }
      }
    }else pinlerterslendi=false;
*/

    //Serial.println("sorunyok");
    //Serial.println("sorunyok2");
    //DHT DHTA(D7, DHT11);



    for (int x = 0; x < pinsayisi + 1; x++) {

      if (pinmode[x] == "OUT" & pinsignaltype[x] == "DIG") {
        pinMode(getGPIOPin(pinname[x]), OUTPUT);
      }


      if (pinmode[x] == "INP" & pinsignaltype[x] == "DIG") {
        pinMode(getGPIOPin(pinname[x]), INPUT);
      }



      if (pinmode[x] == "OUT" & pinsignaltype[x] == "PWM") {

        pinMode(getGPIOPin(pinname[x]), OUTPUT);

        //          pinMode(Pin[b], OUTPUT);
      }

      if (pinmode[x] == "OUT" && pinsignaltype[x] == "SER") {
        // Eğer o pinde eski bir servo nesnesi kalmışsa önce hafızayı tamamen boşaltalım (Re-init koruması)
        if (myservo[x] != NULL) {
          myservo[x]->detach();  // Önce pinden ayır
          delete myservo[x];     // Hafızadan sil
          myservo[x] = NULL;     // İşaretçiyi boşa çıkar
        }

        int8_t fizikselPin = getGPIOPin(pinname[x]);
        pinMode(fizikselPin, OUTPUT);

        // İşte sihirli nesneleştirme satırı: Yeni servo nesnesini dinamik yaratıyoruz!
        myservo[x] = new Servo();
        myservo[x]->attach(fizikselPin);  // Pointer olduğu için '->' kullanılır

        servovar[x] = true;  // Servonun başarıyla kurulduğunu onayla
        Serial.printf_P(PSTR("[SERVO] Pin %s dinamik nesne olarak basariyla kuruldu.\n"), pinname[x].c_str());
      }
    }
        if(rxtxvar)rxtxsetup();
  }
}





int Menu = 0;

String mylocalip;



bool testWifi(void) {
  if (ssid.length() < 2) {
    return false;
  }
  int c1 = 0;
  Serial.println("Con Wifi");
  //display.drawBitmap(0, 0,  Lan_off_logo8x16_glcd_bmp, 16, 8, 1);
  //display.println();
  //display.println("Modeme");
  //display.println("Baglaniyor");
  //display.display();
  Serial.print(ssid + "  " + pass);
  while (c1 < 11) {
    if (WiFi.status() == WL_CONNECTED) {
      //  display.drawBitmap(0, 0,  Lan_on_logo8x16_glcd_bmp, 16, 8, 1);
      //  display.display();
      return true;
    }
    delay(1000);
    Serial.print("*");
    if (LED_BUILTIN_devredisi == false) {
      if (LED_BUILTIN == HIGH) digitalWrite(LED_BUILTIN, LOW);
      else digitalWrite(LED_BUILTIN, HIGH);
    }
    c1++;
  }
  Serial.println("");
  Serial.println("ConWifi timeout,open AP");
  return false;
}


// ip string parçalama için gereken değişkenler
/* 
String liptmp;
String giptmp;
String suptmp;

String lip[4];
String sup[4];
String gip[4];
*/
// ip adreste noktaları virgüle değiştirmek için parçalama metodu başlangıç
/*

    // local ip cfg;
    String liptm=liptmp;
    String lipadrtmp="";
    for(int lips=1;lips<5;lips++){
    lip[lips]=liptm.substring(0,liptm.indexOf("."));
        liptm=liptm.substring(liptm.indexOf(".")+1,liptm.length());
    if (lips<4) lipadrtmp += lip[lips]+",";
    if (lips==4) lipadrtmp += lip[lips];
    }
    IPAddress lipadr=IPAddress().fromString(lipadrtmp);


    String giptm=giptmp;
    String gipadrtmp="";
    for(int gips=1;gips<5;gips++){
    gip[gips]=giptm.substring(0,giptm.indexOf("."));
        giptm=giptm.substring(giptm.indexOf(".")+1,giptm.length());
    if (gips<4) gipadrtmp += gip[gips]+",";
    if (gips==4) gipadrtmp += gip[gips];
    }
    IPAddress gipadr=IPAddress().fromString(gipadrtmp);



    String suptm=suptmp;
    String supadrtmp="";
    for(int sups=1;sups<5;sups++){
    sup[sups]=suptm.substring(0,suptm.indexOf("."));
        suptm=suptm.substring(suptm.indexOf(".")+1,suptm.length());
    if (sups<4) supadrtmp += sup[sups]+",";
    if (sups==4) supadrtmp += sup[sups];
    }
    IPAddress supadr=IPAddress().fromString(supadrtmp);




Serial.println(lipadr);
Serial.println(gipadr);
Serial.println(supadr);
*/
// ip adreste noktaları virgüle değiştirmek için parçalama metodu bitişi


void connectWifi(void) {
  WiFi.mode(WIFI_AP_STA); /*1*/  //ESP8266 works in both AP mode and station mode
  //WiFi.mode(WIFI_STA); /*2*/  // ESP8266 works in station mode
  // WiFi.begin(ssid, password); // given the network

  //    Serial.print(ssid);
  //    Serial.print("connecting to ");
  //    while (WiFi.status() != WL_CONNECTED) {
  //      // not connected to the network
  //    delay(500);
  //    Serial.print(".");
  //  }
  dosyaokussidpass();
  WiFi.hostname(esphostname);
  WiFi.begin(ssid, pass);
  //Serial.println(ssid);
  //Serial.println(pass);
  //delay(999);

  if (testWifi()) {
    Serial.println("Connected!!!");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.gatewayIP());

    //WiFi.hostname(esphostname);
    delay(100);
    WiFi.softAP(esphostname, "12345678");  // bağlanınca ap kalksın için // koyabiliriz.

    IPAddress lip = WiFi.localIP();
    mylocalip = String(lip[0]) + '.' + String(lip[1]) + '.' + String(lip[2]) + '.' + String(lip[3]);

    //buzzercal(2000, 3); delay(100);
    //buzzercal(3000, 2); delay(10);
  } else {

    Serial.println("HotSpot On");
    //                                wifiscan();
    //                                lookAP();// S etup HotSpot


    WiFi.softAP("Esp-" + WiFi.macAddress(), "12345678");
    //delay(100);
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.gatewayIP());
    //buzzercal(3000, 50); delay(100);
    //buzzercal(2500, 70); delay(100);
    //buzzercal(1500, 100); delay(10);
  }

  //firebaseRealtime.begin(FIREBASE_REALTIME_URL, FIREBASE_REALTIME_SECRET);
}



int statusCode;
String st;
void wifiscan(void) {
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("Wifi cihaz yok");
  else {
    st = "[ " + String(n) + " ] Wifi cihaz bulundu<br>";
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      //delay(4);
    }
  }
  Serial.println("");
  //st = "<ol>";
  st += "<ol>";
  for (int i = 0; i < n; ++i) {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);

    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol><br>";
}


/*
bool dhtsensorerror;
float hic, hum, f, t;

 void dhtget(int pinno) {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  DHTA.read();
  delay(20);
  hum = DHTA.readHumidity();
  //f = dht.readTemperature(true);
  //  hum=dht.computeHeatIndex(f, hum);
  // Read temperature as Celsius (the default)
  delay(10);
  t = DHTA.readTemperature();  //
  // Read temperature as Fahrenheit (isFahrenheit = true)


  // Check if any reads failed and exit early (to try again).
  if (isnan(hum) || isnan(t)  // || isnan(f)
  ) {
    Serial.println("Failed to read from DHT sensor!");
    dhtsensorerror = true;
    return;
  } else {
    dhtsensorerror = false;
   
  }
  // Compute heat index in Fahrenheit (the default)
  //  hum = dht.computeHeatIndex(f, hum);
  // Compute heat index in Celsius (isFahreheit = false)
  hic = t;  //
  //hic = dht.computeHeatIndex(hic, hum, false)+hicfark;  // eskiden t idi

  Serial.print(("Humidity: "));
  Serial.print(hum);
  Serial.print(("%  Temperature: "));
  Serial.print(hic);
  Serial.print((" C"));
  Serial.println(".");
}
*/








// Current time
unsigned int xcurrentTime = 0;
// Previous time
// Define timeout time in milliseconds (example: 2000ms = 2s)
const int timeoutTime = 3000;

unsigned long ledsay;



/*
http://192.168.2.97/programkayit?is=

EGER+%28D1.DEGER+>+35.5%29+D3.DEGER%3DHIGH%0D%0AEGER+%28D1.DEGER+<+30.0%29+D3.DEGER%3DLOW
D4%3DD1%2FD2%3BD3*D5%25aaa%3F

%20  :boşluk actionadresi icinde iken
+    :boşluk databloğu içinde iken
%7C  :|
%28  :(
>    :>
%29  :)
%3D  :=
%0D%0A: \r\n  (enter ve satır sonu) karakterleri
%2F  :/
*    :*
%25  :%
%3F  :?
%3B  :;
%26  :&
*/

// float tanımlama için
int floatDegiskenSayisi = 0;
//

// işlemler için
String floatdegiskenismi[27];
float floatdegiskenvalue[27];
int floatdegiskenismisayisi = 27;
String islem;
//



//defi /////////////////////////////////////////////////////////////////////////////////////////
#define MAX_USER 5
String users;
String usernames[MAX_USER];
String passwords[MAX_USER];
int userLevels[MAX_USER];
String usertokn[MAX_USER];
unsigned long userlogouttime[MAX_USER];
String usernamelbl;
String usertoken;


/*
// Textarea'dan veya dosyadan gelen ham metni dizilere dolduran fonksiyon
void kullanicilariYukle(String hamMetin) {
    int userCount = 0;
    
    // Satır satır bölmek için döngü (Her satır bir kullanıcı)
    while (hamMetin.length() > 0 && userCount < MAX_USER) {
        int satirSonu = hamMetin.indexOf("\n");
        String satir = (satirSonu == -1) ? hamMetin : hamMetin.substring(0, satirSonu);
        satir.trim(); // Varsa boşlukları temizle
        
        if (satir.length() > 0) {
            // Satır içindeki virgüllerin yerini bul (Örn: ali,1234,2)
            int ilkVirgul = satir.indexOf(",");
            int ikinciVirgul = satir.lastIndexOf(",");
            
            if (ilkVirgul != -1 && ikinciVirgul != -1 && ilkVirgul != ikinciVirgul) {
                usernames[userCount] = satir.substring(0, ilkVirgul);
                passwords[userCount] = satir.substring(ilkVirgul + 1, ikinciVirgul);
                userLevels[userCount] = satir.substring(ikinciVirgul + 1).toInt(); // 1 veya 2 yapar
                
                userCount++;
            }
        }
        
        // İşlenen satırı metinden at, kalana bak
        if (satirSonu == -1) break;
        hamMetin = hamMetin.substring(satirSonu + 1);
    }
}
defi ///////////////////////////////////////////////////////////////////////////
*/







void dosyaOkuprogram() {
  dosya.close();
  dosya = LittleFS.open("/program.txt", "r");
  if (dosya) {
    programdata = dosya.readString();
    //programdata = programdata.substring(0, programdata.length() - 1);
    Serial.println("PROGRAM DATA v ");
    Serial.println(programdata);
    //Serial.println(gecicipinayar);
    dosya.close();
    if (programdata.indexOf("%0D") > -1) programdata = "";
  } else {
    programdata = " \n";
  }
}


void dosyaYazprogram(String programdatat) {
  reConnectsayac = millis();
  programerrorlog = "";
  // 1. Gelen veriyi önce geçici bir değişkende düzeltiyoruz
  String programdatatmp = Karakterduzeltfunc(programdatat);

  // 2. Güvenlik ve Çakışma Kontrolünü geçici değişken üzerinde yapıyoruz
  if (pinayar.indexOf("D6") != -1 || pinayar.indexOf("D7") != -1) {
    if (programdatatmp.indexOf("RXTX") > -1 && programdatatmp.indexOf("//RXTX") < 0) {
      programerrorlog = "Program hatası: D6, D7 pinayarda kullanılmış! Programda RXTX; kullanılamaz.";
      // "RXTX" ifadesini devre dışı bırakmak için açıklama satırına çevir
      programdatatmp.replace("RXTX", "//RXTX");
    }
  }

  // 3. SADECE veri gerçekten DEĞİŞTİYSE dosyaya ve hafızaya yaz (Flash Ömrünü Korur)
  if (programdatatmp != programdata) {
    programdata = programdatatmp;  // Artık güvenli veriyi ana değişkene atabiliriz

    dosya.close();
    // LittleFS.remove("/program.txt"); // 'w+' modu zaten dosyayı sıfırlar, remove'a gerek yok

    Serial.println(F("Yeni program kaydediliyor..."));
    Serial.println(programdata);

    dosya = LittleFS.open("/program.txt", "w+");
    if (dosya) {
      dosya.print(programdata);
      dosya.close();
    }

    // Sistemi yeni ayarlarla tazele
    setup2();
    if (pinayar.length() > 0 && programdata.length() > 0) {
      programrun();
    }
  } else {
    Serial.println(F("Program verisinde değişiklik yok, yazma atlandı."));
  }
}



//unsigned long mqyolsil10sec;
//unsigned uint8_t mqyolsil10secY[5];
unsigned long aclsil10sec;


String mqc[5];
String mqcyol[5];
String mqyol[5];
String degisenmq[5];
bool mqsendbayrak[5];
int mqdolubayrak;
int mqsay;


String notalar;
uint8_t serstat;

int dhtokusayac = 0;



int sayPtakipicin = 0;

unsigned long lastMillis;

int mqttconnectsayac;
bool mqtterror;

int sil = 0;


void setup() {
  // initialize LED_BUILTIN as an output pin.
  pinMode(LED_BUILTIN, OUTPUT);
  // starttime=millis();
  Serial.begin(115200);
  // dosya setup kısmı ////////////////

  for (int y = 1; y < 13; y++) {
    mqyol[y] = "";
  }


  if (LittleFS.begin()) {
    Serial.println();
    Serial.println("Dosya sistemi başarılı");

    if (sil == 1) {
      LittleFS.remove("/pinayar.txt");
      delay(10);
      LittleFS.remove("/ssidpass.txt");
      delay(10);
      LittleFS.remove("/users.txt");
      delay(10);
      LittleFS.remove("/httpserverip.txt");
      delay(10);
      LittleFS.remove("/usrnamepass.txt");
      delay(10);
      LittleFS.remove("/mqttip.txt");
      delay(10);
      LittleFS.remove("/myssidname.txt");
      delay(10);
      LittleFS.remove("/fben.txt");
      delay(10);
      LittleFS.remove("/fburl.txt");
      delay(10);
      LittleFS.remove("/fbapi.txt");
      delay(10);
      LittleFS.remove("/fbyol.txt");
      delay(10);
      LittleFS.remove("/fbusername.txt");
      delay(10);
      LittleFS.remove("/fbuserpass.txt");
      delay(10);
      LittleFS.remove("/habp.txt");
      delay(10);

      sil = 0;
    }



  } else {
    Serial.println("Dosya sistemi başarısız");
  }

  Serial.println(ESP.getResetReason());
  Serial.println(ESP.getResetInfo());

  // Önce lisans kontrolü (cihazorjinal durumunu belirler)
  dosyaokucihazmac();

  // YENİ EKLEME: Kalıcı aut durumunu dosyadan okuyoruz
  dosyaOkuAutDurum();

  dosyaOkuUzunMetrajMod();

  dosyaokuuprogram();

  dosyaokuacl();
  dosyaokuhabp();

  dosyaokufbyol();
  dosyaokufben();
  dosyaokufburl();
  //dosyaokufbapi();
  //dosyaokufbusername();
  //dosyaokufbuserpass();

  //espLolin();

  dosyaOkupinayar();
  //dosyaokupindurum();


  if (pinayar.length() > 3) {
    dosyaOkuprogram();
    for (int x = 0; x < pinsayisi + 1; x++) {
      if (pinmode[x] == "OUT" & pinsignaltype[x] == "DIG") {
        bool yildizli;
        if (pinlabel[x].indexOf("*") + 1 == pinlabel[x].length()) yildizli = true;
        else yildizli = false;
        if (PinState[x] == "0" || PinState[x] == "") {
          if (yildizli == false) digitalWrite(getGPIOPin(pinname[x]), LOW);
          else digitalWrite(getGPIOPin(pinname[x]), HIGH);
        } else if (PinState[x] == "1") {
          if (yildizli == false) digitalWrite(getGPIOPin(pinname[x]), HIGH);
          else digitalWrite(getGPIOPin(pinname[x]), LOW);
        }
      }
    }
    //pinlerterslendi=true;
  }

  dosyaokumyssidname();

  //htserveroku();
  mqttipoku();
  //delay(2);
  dosyaokussidpass();
  //delay(2);
  // dosya setup kısmı bitti /////

  //httpserver.setNoDelay(true);



  if (WiFi.status() != WL_CONNECTED) connectWifi();


  delay(10);





  httpserver.begin();
  butonactcoloku();
  butonpascoloku();
  butonayrcoloku();
  menutextcoloku();
  butonpbgcoloku();
  Serial.println("Web server Lunched.");


  /*
  if (MDNS.begin("esp8266")) { Serial.println("MDNS responder started"); }
  server.on("/", handleRoot);
  server.on("/gerT", handleger);

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("8080 server started");
*/


  otasetup();
  setup2();
  acltekrar = millis();  // ilk start için
  if (WiFi.status() == WL_CONNECTED) {
    if (habp == -2) dosyaokuhabp();
    if (habp >= 1) {
      mqttipoku();
      MQTTConnect();
      if (MQTTip.length() > 2) MQTTConnect();
    }
  }

  //if (WiFi.status() == WL_CONNECTED) htserveroku();


  if (WiFi.status() == WL_CONNECTED && fben != 0) {
    if (habp == -2) dosyaokuhabp();
  }

  // mDNS sunucusunu esphostname ismiyle baslatiyoruz
  if (MDNS.begin(esphostname)) {
    Serial.println("mDNS sunucusu baslatildi. Adres: http://" + esphostname + ".local");
  }

  /*
  dosyaOkuusers();
Serial.println("usrname okudurm:" + usrname);
Serial.println("usrpaz okudurm:" + usrpaz);
if(usrname.length()<2 || usrpaz.length()<2)
{
  usrname="admin";
  usrpaz="1111";
}
*/
  dosyaOkuusers();



  if (habp == 3) {
    //if(Menu == 0)if (fben!=0 && pinayar.length()>0)updatefbvirtual();
    if (fben != 0 && pinayar.length() > 0 && fbisleniyor == false)
      if (mqttclient.connected() == true) mqttclient.unsubscribe("/" + YOL + "/#");
    updateinput();
    if (Program.length() > 0) programrun();
    updateoutput();
    fbisleniyor = true;
    fbdataguncelle();
    if (mqttclient.connected() == true) mqttclient.subscribe("/" + YOL + "/#");
  }
}



/*
void connectfb() {
  dosyaokufben();
  if (fben == 1) {   // değişecek

    dosyaokufburl();
    //dosyaokufbapi();
    dosyaokufbyol();
    //dosyaokufbusername();
    //dosyaokufbuserpass();

    IPAddress lip = WiFi.localIP();
    if (WiFi.status() == WL_CONNECTED) {
    }
  }
}
*/


void Programtakip(String progdata);

WiFiClient xilent;
bool htpcldepindegisti = false;
int test = 1;




void loop() {
  //Serial.print("Free heap: "); Serial.println(ESP.getFreeHeap());

  //Serial.println(httpserver.status());



  otaloop();






  if (mqttclient.connected() == true) {
    if (habp == 2 && pinayarchg == true && millis() - 10000 > rutingonder10s) {
      IPAddress lip = WiFi.localIP();
      mylocalip = String(lip[0]) + '.' + String(lip[1]) + '.' + String(lip[2]) + '.' + String(lip[3]);

      String mqyol = "/" + YOL + "/FBSERVER";
      String mqdat = "PAYCHG>" + WiFi.macAddress() + "<" + esphostname + ">" + mylocalip;
      rutingonder10s = millis();
      mqttsend(mqyol, mqdat);
    }
  }

  if (mqttclient.connected() == true) {
    if (habp == 2 && habpchg == true && rutingonder10s < millis() - 10000) {
      IPAddress lip = WiFi.localIP();
      mylocalip = String(lip[0]) + '.' + String(lip[1]) + '.' + String(lip[2]) + '.' + String(lip[3]);

      String mqyol = "/" + YOL + "/FBSERVER";
      String mqdat = "PAYNEW>" + WiFi.macAddress() + "<" + esphostname + ">" + mylocalip;
      rutingonder10s = millis();
      mqttsend(mqyol, mqdat);
    }
  }




  //harcananzaman = millis();

  MDNS.update();  // mDNS sorgularini guncellemek icindir



  xilent = httpserver.available();
  if (xilent && fbisleniyor == false) {
    if (habp == 3) timereski = millis() + 4000;
    if (habp == 2) timereski = millis() + 10;
    if (habp == 1) timereski = millis() + 10;
    if (habp == 0) timereski = millis() + 10;

    if (rxtxvar) rxtxloop();

    htpcl(xilent);
    if (htpcldepindegisti == true) {
      pinuygula();
    }
  }

  serin();

/*
  if (millis() - harcananzaman > (test * 200)) {
    Serial.println("htpclde vakit uzadı: ms>" + (String)(millis() - harcananzaman));
    test += 1;
    if (test > 10) test = 1;
  }
*/


  //geciktirmee yavaşlatma 
  if(cihazorjinal == false){delay(1000);}


  if (rescanwifi == true) {
    wifiscan();
    rescanwifi = 0;
  }

  //harcananzaman = millis();

  /////////////////// 5 saniyede bir acl ilanı yap
  if (mqttclient.connected()) {
    mqttclient.loop();

    if (ACL != "9" && ACLilanciyim == true && acltekrar < millis()) {
      acltekrar = millis() + 10000;
      String myol = "/" + YOL + "/" + esphostname;
      mqttsend(myol, "/" + YOL + "/ALLDEV=ACL:" + ACL);
    }

    ///////////////// ACL sormak için
    if (ACL == "9") {
      updateinput();
      programrun();
      if (ACLilanciyim == false) {
        if (aclsor < millis()) {
          aclsor = millis() + 10000;
          String myol = "/" + YOL + "/" + esphostname;
          String fulldata = "/ALLDEV=ACIL:" + WiFi.macAddress() + "<" + esphostname + ">" + mylocalip;
          mqttsend(myol, "/" + YOL + fulldata);
        }
      }
    }
    ////////////////
  } else {
    mqtterror = true;
  }
  ////////////////



  if (WiFi.status() != WL_CONNECTED) {
    if (millis() - reConnectsayac > 30000) {
      reConnectsayac = millis();
      connectWifi();
      if (WiFi.status() == WL_CONNECTED) {

        if (habp > 0) {
          if (MQTTip.length() > 2) MQTTConnect();
        }
      }
    }
  }




  if (millis() - ledsay > 880) {
    if (millis() - ledsay == 881) {
      digitalWrite(LED_BUILTIN, LOW);
    }
    if (millis() - ledsay > 1000) {
      ledsay = millis();
      digitalWrite(LED_BUILTIN, HIGH);


      // int chk = DHTA.read();


      // pin işlemleri rutini bitiş ////////////////////////////
    }
  }

  for (int t = 0; t < MAX_USER + 1; t++) {
    if (usertokn[t] != "" && userLevels[t] == 2) {
      if (millis() - userlogouttime[t] > logintimeoutmax) {
      }
    }
  }



  if (habp == -2) dosyaokuhabp();
  if (habp >= 1) {
    if (mqtterror == true && MQTTip.length() > 1) {
      if (WiFi.status() == WL_CONNECTED) {
        if (!mqttclient.connected()) {
          if (mqttconnectsayac < 5001) MQTTConnect();
          if (mqttconnectsayac >= 5000) mqttconnectsayac += 1;
          if (mqttconnectsayac > 10000) {
            mqttconnectsayac = 0;
            mqtterror = true;
          }  //20000 den büyükse baştan dene
        }
      }
    }
  }

  //if (millis() - harcananzaman > 100) Serial.println("mqttclient.loop vaik uzadı: ms>" + (String)(millis() - harcananzaman));



  //if(htyolla != "")httpgonder();



  int upd;

  if (habp == 0) upd = 100;
  if (habp == 1) upd = 100;
  if (habp == 2) upd = 100;
  if (habp == 3) upd = 500;






  if ((millis() - timereski) % upd == 0 && fbisleniyor == false) {
    headerold = "";
    //harcananzaman = millis();
    if (millis() - timereski > 5000) timereski = millis();
    if (pinayar.length() > 0) updateinput();
    if (pinayar.length() > 0 && programdata.length() > 0) programrun();
    if (pinayar.length() > 0) updateoutput();
    if (pinayar.length() > 0) vrkontrol();
    //if (millis() - harcananzaman > 100) Serial.println("update vr prog vaik uzadı: ms>" + (String)(millis() - harcananzaman));
  }


  if (habp > 0 && habp < 3) {
    if (habp == 2) {
      if (psco == true || psci == true || htpcldepindegisti == true) {
        String mqyo = "/" + YOL + "/FBSERVER";
        String mqda = "FBSERVER>" + esphostname + "pin";

        if (ACLilanciyim == true) { mqda += "ACL:" + String(ACL) + ","; }

        for (int hh = 0; hh < 11; hh++) {
          if (pinname[hh] != "") {
            mqda += pinname[hh] + ":" + PinState[hh] + ",";
          }
        }
        mqttsend(mqyo, mqda);
        psci = false;
        psco = false;
      }
    }
  }

  if (habp == 3) {
    if (millis() - timereski >= 5000) {
      timereski = millis();
      //if(Menu == 0)if (fben!=0 && pinayar.length()>0)updatefbvirtual();
      if (fben != 0 && pinayar.length() > 0 && ACL.toInt() != 9 && fbisleniyor == false) {
        //updateinput();
        //programrun();
        //updateoutput();
        mqttclient.unsubscribe("/" + YOL + "/#");
        fbisleniyor = true;
        fbsayacoku();
        mqttclient.subscribe("/" + YOL + "/#");
      }
    }
  }
}


void pinuygula() {

  if (!cihazorjinal) return;

  if (programdata.length() > 4) programrun();

  for (int x = 0; x < pinsayisi + 1; x++) {


    if (pinmode[x] == "OUT") {
      String pinstatesakla;
      if (acildeyim[x] == true) {
        pinstatesakla = PinState[x];
        PinState[x] = acildeger[x];
      }

      if (pinmode[x] == "OUT" && pinsignaltype[x] == "DIG") {
        bool yildizli;
        if (pinlabel[x].indexOf("*") + 1 == pinlabel[x].length()) yildizli = true;
        else yildizli = false;
        if (PinState[x] == "0.00" || PinState[x] == "0" || PinState[x] == "LOW" || PinState[x] == "OFF") {
          if (yildizli == false) digitalWrite(getGPIOPin(pinname[x]), LOW);
          else digitalWrite(getGPIOPin(pinname[x]), HIGH);
          //Serial.println("yazdım::: " + pinname[x] + ":" + PinState[x]);
        }

        if (PinState[x] == "1.00" || PinState[x] == "1" || PinState[x] == "HIGH" || PinState[x] == "ON") {
          if (yildizli == false) digitalWrite(getGPIOPin(pinname[x]), HIGH);
          else digitalWrite(getGPIOPin(pinname[x]), LOW);
          //Serial.println("yazdım::: " + pinname[x] + ":" + PinState[x]);
        }
      }

      if (pinmode[x] == "OUT" && pinsignaltype[x] == "SER") {
        if (servovar[x] && myservo[x] != NULL) {
          // HANGİ DEĞERİN GELDİĞİNİ GÖRMEK İÇİN DEBUG LOGU EKLEYİN:
          //Serial.printf_P(PSTR("[DEBUG] Servo tetiklendi! Gelen string: '%s'\n"), PinState[x].c_str());

          int aciDeperi = constrain(PinState[x].toInt(), 0, 180);

          //Serial.printf_P(PSTR("[DEBUG] Motora yazilan aci: %d\n"), aciDeperi);

          myservo[x]->write(aciDeperi);

          psco = true;
          ePinState[x] = PinState[x];
        }
      }


      if (pinmode[x] == "OUT" && pinsignaltype[x] == "PWM") {
        int PWMdegerint = PinState[x].toInt();
        Outpwm(pinname[x], PWMdegerint);
      }

      if (ePinState[x] != PinState[x]) {
        // 1. DURUM: SERVO MOTOR KONTROLÜ (Nesne Kontrollü Güvenli Sürüş)
        // myservo[x] != NULL şartı sayesinde, nesne yaratılmadıysa asla koda girmez ve çökme yaşanmaz!
        if (servovar[x] && myservo[x] != NULL) {
          int aciDeperi = constrain(PinState[x].toInt(), 0, 180);

          // 🎯 Pointer nesnesi olduğu için artık '.' yerine '->' kullanıyoruz:
          myservo[x]->write(aciDeperi);

          psco = true;
          ePinState[x] = PinState[x];
        }
        // 2. DURUM: SENSÖR VE DİĞER SİNYAL TİPLERİ...
        // ... (Geri kalan kodlarınız aynen devam ediyor) ...

        if (acildeyim[x] == true) {
          PinState[x] = pinstatesakla;
        }
      }
    }
  }
  if (htpcldepindegisti == true) htpcldepindegisti = false;
}

void vrkontrol() {

  for (int vr = 0; vr < 6; vr++) {
    if (VRP[vr].length() > 0) {
      Serial.println("VR" + (String)vr + ": " + VRP[vr]);
      if (VRP[vr].indexOf("BUZ>") == 0) {
        notalar = VRP[vr].substring(VRP[vr].indexOf(":") + 1, VRP[vr].length());
        notalar = notalar.substring(notalar.indexOf("BUZ>") + 4, notalar.length());
        play(notalar);
        if (bestursay == 0) VRP[vr] = "";
      }

      /*      if (VRP[vr].indexOf("SEND%3E") == 0) {
        String htpServerip = VRP[vr].substring(VRP[vr].indexOf("%3E") + 3, VRP[vr].length());
        htpServerip = htpServerip.substring(0, htpServerip.indexOf(","));
        String gonderilecek = htpServerip.substring(htpServerip.indexOf(",") + 1, htpServerip.length());
        gonderilecek = htpServerip.substring(0, htpServerip.length());
        sendserver80(htpServerip, "80" ,gonderilecek);
      }
      */
    }
  }
}




void Outpwm(String pinismi, int PWMdegerint) {

  //Serial.println("GELDİM");
  //Serial.println(pinismi);
  if (pinismi == "") return;
  int pinismiint;
  if (pinismi.length() > 1)
    pinismiint = pinismi.substring(1, 2).toInt();
  else pinismiint = pinismi.toInt();
  if (pinismi.indexOf("A") > -1) pinismiint += 9;

  analogWrite(getGPIOPin(pinismi), PWMdegerint);
  PinState[pinismiint] = PWMdegerint;
}











/*
String girdi;
String userstmp;
void MQTTuserYuzdeliifadesil(){
if(users.length()>0){
  userstmp=users;
  Serial.println("users ayarlanıyor:");
  Serial.println(users);
girdi="";
for (int j=1;j<userstmp.length();j++){
girdi += userstmp.substring(0,userstmp.indexOf("%7C")) + "|";
userstmp= userstmp.substring(userstmp.indexOf("%7C")+3,userstmp.length());

girdi += userstmp.substring(0,userstmp.indexOf("%0D%0A")) + "\n";
userstmp= userstmp.substring(userstmp.indexOf("%0D%0A")+6,userstmp.length());

if(userstmp.length()<1) break;
}

  girdi=girdi.substring(0,girdi.indexOf("%"));
  users=girdi;
  }
}
*/



String Karakterduzeltfunc(String gelent) {
  Serial.println(F("Karakterduzelte girdim"));
  Serial.println(gelent);

  // 1. ADIM: Satır sonlarını (\r\n) en başta düzeltiyoruz ki metin satırlara düzgün ayrılsın
  gelent.replace(F("%0D%0A"), F("\n"));
  gelent.replace(F("+"), F(" "));
  gelent.replace(F("%20"), F(" "));

  // 2. ADIM: Standart Sembol Çözümleri
  gelent.replace(F("%26"), F("&"));
  gelent.replace(F("%28"), F("("));
  gelent.replace(F("%29"), F(")"));
  gelent.replace(F("%7C"), F("|"));  // Dik çizgiler temizleniyor
  gelent.replace(F("%3B"), F(";"));
  gelent.replace(F("%3D"), F("="));
  gelent.replace(F("%3F"), F("?"));
  gelent.replace(F("%3E"), F(">"));
  gelent.replace(F("%3C"), F("<"));
  gelent.replace(F("%7B"), F("{"));
  gelent.replace(F("%7D"), F("}"));
  gelent.replace(F("%5B"), F("["));
  gelent.replace(F("%5D"), F("]"));
  gelent.replace(F("%2B"), F("+"));
  gelent.replace(F("%21"), F("!"));
  gelent.replace(F("%22"), F("\""));
  gelent.replace(F("%3A"), F(":"));  // İki noktalar temizleniyor
  gelent.replace(F("%23"), F("#"));
  gelent.replace(F("%27"), F("'"));
  gelent.replace(F("%2C"), F(","));
  gelent.replace(F("%2F"), F("/"));

  // 3. ADIM: GÜVENLİ BÖLGE (Tüm sembol işlemleri bitti, şimdi gerçek yüzde işaretlerini çözebiliriz)
  gelent.replace(F("%25"), F("%"));

  // 4. ADIM: Türkçe Karakter Düzeltmeleri
  gelent.replace(F("%C5%9E"), F("Ş"));
  gelent.replace(F("%C5%9F"), F("ş"));
  gelent.replace(F("%C3%87"), F("Ç"));
  gelent.replace(F("%C3%A7"), F("ç"));
  gelent.replace(F("%C3%96"), F("Ö"));
  gelent.replace(F("%C3%B6"), F("ö"));
  gelent.replace(F("%C3%9C"), F("Ü"));
  gelent.replace(F("%C3%BC"), F("ü"));
  gelent.replace(F("%C4%9E"), F("Ğ"));
  gelent.replace(F("%C4%9F"), F("ğ"));
  gelent.replace(F("%C4%B1"), F("ı"));
  gelent.replace(F("%C4%B0"), F("İ"));

  Serial.print(F("Düzeltme sonucu: "));
  Serial.println(gelent);
  return gelent;
}

/*
String Karakterduzeltfunc(String gelent){

String gelentt;
String gidens;
String gidentt;
String sdf;
String sdf2;
int yuzdemi;
int artimi;

String karakter[38];
String yerinegec[38];

 karakter[0] ="+";   //    :boşluk databloğu içinde iken
          yerinegec[0] = " ";
 karakter[1] ="%20"; //  :boşluk actionadresi icinde iken
          yerinegec[1] = " ";
 karakter[2] ="%25"; //  :%
          yerinegec[2] = "%";
 karakter[3] ="%26"; //  :&
          yerinegec[3] = "&";
 karakter[4] ="%28"; //  :(
          yerinegec[4] = "(";
 karakter[5] ="%29"; //  :)
          yerinegec[5] = ")";
 karakter[6] ="%7C"; //  :|
          yerinegec[6] = "|";
 karakter[7] ="%2F"; //  :/
          yerinegec[7] = "/";
 karakter[8] ="%3B"; //  :;
          yerinegec[8] = ";";
 karakter[9] ="%3D"; //  :=
          yerinegec[9] = "=";
 karakter[10] ="%3F"; //  :?
          yerinegec[10] = "?";
 karakter[11] ="%3E"; //  :>
          yerinegec[11] = ">";
 karakter[12] ="%3C"; //  :<
          yerinegec[12] = "<";
 karakter[13] ="%7B"; //  :{
          yerinegec[13] = "{";
 karakter[14] ="%7D"; //  :}
          yerinegec[14] = "}";
 karakter[15] ="%5B"; //  :[
          yerinegec[15] = "[";
 karakter[16] ="%5D"; //  :]
          yerinegec[16] = "]";
 karakter[17] ="%2B"; //  :+
          yerinegec[17] = "+";
//         - * /  aynı
 karakter[18] ="%21"; //  :!
          yerinegec[18] = "!";

  karakter[19] ="%0D%0A"; //: \r\n  (enter ve satır sonu) karakterleri
        yerinegec[19] = "\n";

  karakter[20] ="%22"; 
        yerinegec[20] = "\"";

  karakter[21] ="%3A";  
        yerinegec[21] = ":";

  karakter[22] ="%3B";  
        yerinegec[22] = "\"";

  karakter[23] ="%23";  
        yerinegec[23] = "#";

  karakter[24] ="%27";  
        yerinegec[24] = "'";

  karakter[25] ="%2C";  
        yerinegec[25] = ",";

  karakter[26] ="%C5%9E";  
        yerinegec[26] = "Ş";

  karakter[27] ="%C5%9F";  
        yerinegec[27] = "ş";

  karakter[28] ="%C3%87";  
        yerinegec[28] = "Ç";
        
  karakter[29] ="%C3%A7";  
        yerinegec[29] = "ç";

  karakter[30] ="%C3%96";  
        yerinegec[30] = "Ö";
        
  karakter[31] ="%C3%B6";  
        yerinegec[31] = "ö";

  karakter[32] ="%C3%9C";
        yerinegec[32] = "Ü";
        
  karakter[33] ="%C3%BC"; 
        yerinegec[33] = "ü";

  karakter[34] ="%C4%9E";  
        yerinegec[34] = "Ğ";
        
  karakter[35] ="%C4%9F";  
        yerinegec[35] = "ğ";

  karakter[36] ="%C4%B1";  
        yerinegec[36] = "ı";
        
  karakter[37] ="%C4%B0";  
        yerinegec[37] = "İ";



gelentt = gelent;
gidens="";
  for(int y=0;y<gelentt.length()+1;y++){
    if(gelentt.length()<1)break;
     yuzdemi=10000;
     artimi=10000;

     if(gelentt.indexOf("%")>-1){yuzdemi=gelentt.indexOf("%");}
     if(gelentt.indexOf("+")>-1){artimi=gelentt.indexOf("+");}
     if(yuzdemi<artimi)
     {
           sdf="";
           sdf2="";
          gidentt += gelentt.substring(0,yuzdemi);
          sdf = gelentt.substring(yuzdemi,yuzdemi+3);
          sdf2 = gelentt.substring(yuzdemi,yuzdemi+6);
          
          if(sdf2=="%0D0A")sdf="%0D%0A";
          if(sdf2=="%C5%9E")sdf="%C5%9E";
          if(sdf2=="%C5%9F")sdf="%C5%9F";  
          if(sdf2=="%C3%87")sdf="%C3%87";  
          if(sdf2=="%C3%A7")sdf="%C3%A7";  
          if(sdf2=="%C3%96")sdf="%C3%96";  
          if(sdf2=="%C3%B6")sdf="%C3%B6";  
          if(sdf2=="%C3%9C")sdf="%C3%9C";  
          if(sdf2=="%C3%BC")sdf="%C3%BC";  
          if(sdf2=="%C4%9E")sdf="%C4%9E";  
          if(sdf2=="%C4%9F")sdf="%C4%9F";  
          if(sdf2=="%C4%B1")sdf="%C4%B1";  
          if(sdf2=="%C4%B0")sdf="%C4%B0";  


          for (int c=1;c<38;c++)
          {
              if(karakter[c].indexOf(sdf)>-1)
              {
              gidentt += yerinegec[c];
              gelentt = gelentt.substring(yuzdemi+sdf.length(),gelentt.length());
              y=yuzdemi+sdf.length();
              break; 
              }
          }
     }

     if(artimi<yuzdemi)
          {
            gidentt += gelentt.substring(0,artimi);

              gidentt += yerinegec[0];
              gelentt = gelentt.substring(artimi+1,gelentt.length());
              y=artimi;
          }
     

     if(artimi==yuzdemi)
          {
            gidentt += gelentt;
            break;
          }
          //Serial.println(gelentt);
     }

      gelentt="";
      sdf="";

      String gidenrn=gidentt;
      gidentt="";
      //Serial.println(gidens);
      gidens="";
      Serial.println(gidenrn);

    for(int x=0;x<gidenrn.length()+1;x++)
    {
      if(gidenrn.indexOf("\n\n")>-1)
      {
        gidens += gidenrn.substring(0, (gidenrn.indexOf("\n\n"))) + "\n";
            gidenrn=gidenrn.substring(gidenrn.indexOf("\n\n")+2,gidenrn.length());
      } else 
      {
        gidens += gidenrn.substring(0, gidenrn.length());
        break;
      }
    }
      String gidenstm=gidens;
      Serial.println(gidens);
      return gidens;
      

  }
*/