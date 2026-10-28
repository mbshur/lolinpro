//
void slaveRxvoid() {
  // Donanımsal porta veri geldi mi?
  if (Serial.available() > 0) {
    // Satır sonu karakterine (\n) kadar oku
    String gelenVeri = Serial.readStringUntil('\n');
    gelenVeri.trim();
    Serial.println(" gelen ham veri : " + gelenVeri);
    if (gelenVeri.length() > 0) {
      CHZisimkontrol(gelenVeri);
    }
  }
}

  //
  void slaveTxvoid() {
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
      Serial.println("ismim: " + CHZadi);

      // 🎯 PİNLERİ OLUŞTURMA KÖPRÜSÜ
      int pBasi = gelenVeri.indexOf("[");
      int pSonu = gelenVeri.indexOf("]");
      if (pBasi > -1 && pSonu > pBasi) {
        String icerikMetni = gelenVeri.substring(pBasi + 1, pSonu);
        CHZtanimla(icerikMetni); 
      }
    }
    return;
  }
  else { // cihaz adı zaten öğrenilmiş, komutları dinle
    if (gelenVeri.indexOf(CHZadi + ":") > -1) {
      int isimbasi = gelenVeri.indexOf(CHZadi + ":");
      int veribasi = gelenVeri.indexOf(CHZadi + ":") + CHZadi.length() + 1;
      String gelenverikesi = gelenVeri.substring(veribasi, gelenVeri.length());
      int benimgelenveriSonum = gelenverikesi.indexOf(";");
      
      rxdata = gelenverikesi.substring(0, benimgelenveriSonum);
      
      txdata = gelenVeri.substring(0, isimbasi) + gelenverikesi.substring(benimgelenveriSonum + 1);
      txdata.trim();
      
      if (txdata != "") Serial.println("txdata:" + txdata);
      else { Serial.println("tx ile gönderilecek data yok"); }
    } 
    else {
      txdata = gelenVeri;
      if (txdata != "") Serial.println("txdata:" + txdata);
      else { Serial.println("tx ile gönderilecek data yok"); }
    }
  }
}
