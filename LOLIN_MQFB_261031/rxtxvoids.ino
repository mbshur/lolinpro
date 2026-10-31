#include <SoftwareSerial.h>



SoftwareSerial unoSerial;

void rxtxsetup() {
  rxtxvar = true;
  const int8_t ESP_RX_PIN = D0;  // Uno'nun TX'inden (direnç korumalı) gelen hat
  const int8_t ESP_TX_PIN = D2;  // Uno'nun RX'ine giden hat
  //const uint32_t BAUD_RATE = 19200;
  const uint32_t BAUD_RATE = 38400;
  // Software Serial pin yapılandırması ve 19200 baud başlatılması
  // (RX, TX) parametreleri sırasıyla verilir
  // Yazılımsal portu sabit pinlerimiz ve 19200 baud ile başlatıyoruz
  unoSerial.begin(BAUD_RATE, SWSERIAL_8N1, ESP_RX_PIN, ESP_TX_PIN, false);

  Serial.println(F("Sistem Hazır. D2(RX) ve D0(TX) üzerinden 57200 baud ile dinleniyor..."));
}

void rxtxloop() {
  if (unoSerial.available()) {
    String gelenHat = unoSerial.readStringUntil('\n'); 
    gelenHat.trim(); 

    // 1. Önce sensör paketlerini emen motorumuz (:AN:)
    if (gelenHat.indexOf(":AN:") > -1) {
      espSensorPaketiIsle(gelenHat); 
    }

    // 🎯 2. ADIM: AKILLI ARIZA / ACİL STOP TEŞHİS MOTORU (:ERR:)
    if (gelenHat.indexOf(":ERR:") > -1) {
      // Gelen paketi parçala. Örn: "UNO1:ERR:SW1=1;"
      // Önce sonundaki ";" işaretini temizleyelim
      if (gelenHat.endsWith(";")) gelenHat = gelenHat.substring(0, gelenHat.length() - 1);

      int ilkIkiNokta = gelenHat.indexOf(':');
      int ikinciIkiNokta = gelenHat.indexOf(":ERR:");

      if (ilkIkiNokta > 0 && ikinciIkiNokta > ilkIkiNokta) {
        String cihaz = gelenHat.substring(0, ilkIkiNokta); // Örn: "UNO1"
        String arizaDetayi = gelenHat.substring(ikinciIkiNokta + 5); // Örn: "SW1=1"
        cihaz.trim(); cihaz.toUpperCase(); arizaDetayi.trim();

        // 🚨 SİSTEMİ ANINDA ACİL DURDURMAYA ALIYORUZ!
        Serial.println(F("\n=================================================="));
        Serial.printf("🚨 [ACİL STOP SİSTEMİ] %s Cihazından Hata Sinyali Alındı!\n", cihaz.c_str());
        Serial.printf("⚠️  Tetiklenen Anahtar/Arıza Detayı: %s\n", arizaDetayi.c_str());
        
        // Bu cihaza gönderilen en son emri de sistem teşhisi için ekrana basıyoruz
        for (int s = 0; s < 10; s++) {
          if (slaves[s] == cihaz) {
            Serial.printf("--> Gönderilen Son Emir: %s\n", slavestx[s].c_str());
            slavesrx[s] = gelenHat; // Durumu hafızaya da yedekle (Web arayüzü için)
            break;
          }
        }
        Serial.println(F("==================================================\n"));

        // Sihirli durdurma fonksiyonumuzu çağırarak uzun metrajı imha ediyoruz
        ualtprogdur();
        return; // İşlem bitti, fonksiyonu sonlandır
      }
    }

    // 3. Adım: Sizin eski düz ERR veya diğer durumları paylaşan standart eşleştirme bloğunuz (Eski uyumluluk için)
    if (gelenHat.indexOf(":") > 0 && gelenHat.indexOf(":AN:") == -1 && gelenHat.indexOf(":ERR:") == -1) {
      String cihaz = gelenHat.substring(0, gelenHat.indexOf(":"));
      String durum = gelenHat.substring(gelenHat.indexOf(":") + 1);
      cihaz.trim(); cihaz.toUpperCase(); durum.trim();

      for (int s = 0; s < 10; s++) {
        if (slaves[s] == cihaz) {
          slavesrx[s] = gelenHat;
          if (durum.equalsIgnoreCase("ERR")) {
            Serial.printf("[SİSTEM TEŞHİS] %s Cihazı Çöktü! Son Emir: %s\n", cihaz.c_str(), slavestx[s].c_str());
            ualtprogdur();
          }
          break;
        }
      }
    }
  }
}

void txsend(String txveri) {

  String txv = txveri;
  for (int tv = 0; tv < 10; tv++) {
    if (txv.indexOf(slaves[tv] + ":") > -1 && slaves[tv].length() > 0) {
      int slaveveribasi = txv.indexOf(slaves[tv] + ":") + slaves[tv].length() + 1;
      String gidenverikesi = txv.substring(slaveveribasi, txv.length());
      int benimgidenveriSonum = gidenverikesi.indexOf(";");
      slavestx[tv] = gidenverikesi.substring(0, benimgidenveriSonum);
      //Serial.print("slaveno:");Serial.print(tv);Serial.print(" slaveismi:");Serial.print(slaves[tv]);
      //Serial.print(" slavetx:");Serial.println(slavestx[tv]);
    }
  }
  txv = "";
  if (txveri.lastIndexOf(";") + 1 != txveri.length()) txveri += ";";
  Serial.println("txveri gönderildi. Veri:" + txveri);
  unoSerial.println(txveri);
}



void espSensorPaketiIsle(String gelenHatVerisi) {
  // Örnek Girdi: "UNO1:AN:POT1=411|SENS2=1023;"
  gelenHatVerisi.trim();

  // Sonundaki noktalı virgülü temizle
  if (gelenHatVerisi.endsWith(";")) {
    gelenHatVerisi = gelenHatVerisi.substring(0, gelenHatVerisi.length() - 1);
  }

  int anIdx = gelenHatVerisi.indexOf(":AN:");
  if (anIdx == -1) return;  // Eğer analog bildirim paketi değilse çık

  // ":AN:" kısmından sonrasını ayıkla (Örn: "POT1=411|SENS2=1023")
  String icerik = gelenHatVerisi.substring(anIdx + 4);
  icerik.trim();

  int baslangic = 0;
  while (baslangic < icerik.length()) {
    int sonrakiBoru = icerik.indexOf('|', baslangic);
    if (sonrakiBoru == -1) sonrakiBoru = icerik.length();

    // Tek bir sensör hücresini ayıkla (Örn: "POT1=411")
    String hücre = icerik.substring(baslangic, sonrakiBoru);
    hücre.trim();
    baslangic = sonrakiBoru + 1;

    if (hücre == "") continue;

    int esitIdx = hücre.indexOf('=');
    if (esitIdx > -1) {
      String sensörAdi = hücre.substring(0, esitIdx);
      String sensörDegerStr = hücre.substring(esitIdx + 1);
      sensörAdi.trim();
      sensörDegerStr.trim();

      if (sensörAdi != "") {
        float deger = sensörDegerStr.toFloat();  // Gelen değeri sayıya çevir

        // 🎯 SİHİRLİ DOKUNUŞ: Değişkeni canlı sözlüğe yaz/güncelle!
        esp_canli_degiskenler[sensörAdi] = deger;

        // İsteğe bağlı: Değişimleri takip etmek için log basabilirsiniz
        Serial.printf("[ESP-Hafiza] 📈 Değişken Güncellendi -> %s = %.2f\n", sensörAdi.c_str(), deger);
      }
    }
  }
}