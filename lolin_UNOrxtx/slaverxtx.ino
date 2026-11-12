
void slaveRxvoid() {
  rxBuffer.reserve(100);
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n') {
      rxBuffer.trim();

      //Serial.print(F(" gelen ham veri : ")); // F() makrosu eklendi
      //Serial.println(rxBuffer);

      if (rxBuffer.length() > 0) {
        CHZisimkontrol(rxBuffer);  // Doğrudan rxBuffer'ı gönderiyoruz, ekstra String oluşturmuyoruz
      }

      rxBuffer = "";  // İşlem bitince hemen temizle
    } else if (c != '\r') {
      rxBuffer += c;
    }
  }
}

//
void slaveTxvoid() {
  if (txdata.length() > 0) {
    Serial.println(txdata);
    txdata = "";
  }
}

//
// --- 🌐 HABERLEŞME VE ISI KONTROL MOTORU ---
void CHZisimkontrol(String gelenVeri) {
  // Cihaz adı boş ise cihaz adı tanımlama gelmiş ise cihaz adını al
  if (CHZadi == "") {
    int isimbasi = -1;
    int isimsonu = -1;
    if (gelenVeri.indexOf("DEF<") > -1) isimbasi = gelenVeri.indexOf("DEF<") + 4;
    if (gelenVeri.indexOf(">") > -1) isimsonu = gelenVeri.indexOf(">");
    if (isimbasi > -1 && isimsonu > -1) {
      CHZadi = gelenVeri.substring(isimbasi, isimsonu);
      CHZadi.trim();
      //Serial.println("ismim: " + CHZadi);

      // 🎯 PİNLERİ OLUŞTURMA KÖPRÜSÜ
      int pBasi = gelenVeri.indexOf("[");
      int pSonu = gelenVeri.indexOf("]");
      if (pBasi > -1 && pSonu > pBasi) {
        String icerikMetni = gelenVeri.substring(pBasi + 1, pSonu);
        txdata = "........";
        slaveTxvoid();
        CHZtanimla(icerikMetni);
        //txdata = gelenVeri.substring(gelenVeri.indexOf("DEF<"), pSonu);
        if (txdata != "") {  //Serial.println("txdata:" + txdata);slaveTxvoid();
        } else {             //Serial.println("tx ile gönderilecek data yok");
        }
      }
    }
    return;
  } else {
    // cihaz adı zaten öğrenilmiş, fakat yine tanıımlama geldiyse kendine ait kısmı sil
    if (gelenVeri.indexOf("DEF<" + CHZadi) > -1) {
      gelenVeri = "";
      return;
      /*
      int silbasi = gelenVeri.indexOf("DEF<" + CHZadi);
      String gelenveritmp = gelenVeri.substring(silbasi + 4, gelenVeri.length());
      int silsonu = -1;
      if (gelenveritmp.indexOf("]") > -1) silsonu = gelenveritmp.indexOf("]");
      gelenVeri = gelenVeri.substring(0, silbasi) + gelenVeri.substring(silsonu, gelenVeri.length());
*/
    }

    if (gelenVeri.indexOf("DURX") > -1) {
      // Güvenlik koruması: Tüm step motorları anında durdur
      for (int m = 0; m < toplamStep; m++) {
        digitalWrite(stepPin1[m], LOW);
        digitalWrite(stepPin2[m], LOW);
        stepKalanAdim[m] = 0;  // 🚨 İŞTE TAM BURAYA EKLEYİN! (Arka plan adımlarını da sıfırlar)
      }

      // Tüm dijital çıkışları (Röleleri vb.) emniyet için kapat
      for (int r = 0; r < toplamDo; r++) {
        digitalWrite(doPin[r], LOW);
      }

      for (int r = 0; r < toplamServo; r++) {
        if (!servoBaglandi[r]) continue;
        servoHedefAci[r] = servoAnlikAci[r];
        servoNesneleri[r].detach();  // PWM sinyalini keser, motor boşa çıkar
        servoBaglandi[r] = false;    // Tekrar çalıştırırken attach() yapmanız gerekir
      }

      for (int r = 0; r < MAX_MODUL; r++){
        analogRapor[r]=false;
        diRapor[r]=false;
      }
    }

    // cihaz adı zaten öğrenilmiş, komutları dinle
    if (gelenVeri.indexOf(CHZadi + ":") > -1) {
      int isimbasi = gelenVeri.indexOf(CHZadi + ":");
      int veribasi = gelenVeri.indexOf(CHZadi + ":") + CHZadi.length() + 1;
      String gelenverikesi = gelenVeri.substring(veribasi, gelenVeri.length());
      int benimgelenveriSonum = gelenverikesi.indexOf(";");

      rxdata = gelenverikesi.substring(0, benimgelenveriSonum);
      ////Serial.println("rxdata--" + rxdata);
      txdata = gelenVeri.substring(0, isimbasi) + gelenverikesi.substring(benimgelenveriSonum + 1);
      txdata.trim();

      if (txdata != "") {  //Serial.println("txdata:" + txdata);
      } else {             //Serial.println("tx ile gönderilecek data yok");
      }
    } else {
      txdata = gelenVeri;
      if (txdata != "") {  //Serial.println("txdata:" + txdata);
      } else {             //Serial.println("tx ile gönderilecek data yok");
      }
    }
  }
}
